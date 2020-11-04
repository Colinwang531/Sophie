#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/make_shared.hpp"
#include "boost/pointer_cast.hpp"
#ifdef _WINDOWS
#include "glog/log_severity.h"
#include "glog/logging.h"
#else
#include "glog/log_severity.h"
#include "glog/logging.h"
#endif//_WINDOWS
#include "Error.h"
#include "Define.h"
#include "ASIO/TCPConnector.h"
using TCPConnector = base::network::TCPConnector;
using TCPConnectorPtr = boost::shared_ptr<TCPConnector>;
#include "Protocol/DataPhrase.h"
using DataParser = base::protocol::DataParser;
using DataPacker = base::protocol::DataPacker;
#include "Protocol/ComponentPhrase.h"
#include "Protocol/DevicePhrase.h"
#include "Protocol/EventPhrase.h"
#include "Xml/XmlCodec.h"
using XMLParser = base::xml::XMLParser;
using XMLPacker = base::xml::XMLPacker;
#include "Component/AbstractComponent.h"
using AbstractComponent = base::component::AbstractComponent;
#include "Packet/Message/MessagePacket.h"
using DataPacket = base::packet::DataPacket;
using DataPacketPtr = boost::shared_ptr<DataPacket>;
using MessagePacket = base::packet::MessagePacket;
using MessagePacketPtr = boost::shared_ptr<MessagePacket>;
#include "HKDComponentClient.h"

HKDComponentClient::HKDComponentClient(
	const std::string address,
	const unsigned short port /* = 60531 */)
	: /*AbstractMediaStreamClient(address, port)*/ AbstractWorker(), mediaIP{ address }, mediaPort{ port }
{}
HKDComponentClient::~HKDComponentClient() {}

int HKDComponentClient::createNewClient(const std::string address)
{
	int e{ !address.empty() ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		MajordomoWorkerPtr mdwp{
			boost::make_shared<MajordomoWorker>(
				boost::bind(&HKDComponentClient::afterPolledDataFromWorkerCallback, this, _1, _2, _3, _4, _5)) };
		if (mdwp && eSuccess == mdwp->startWorker(address))
		{
			worker.swap(mdwp);
			//在客户端注册或心跳之前创建UUID标识
			AbstractWorker::generateUUIDWithName("HKD");
			e = AbstractWorker::createNewClient("");
			asioService.startService();
		}
		else
		{
			e = eBadNewObject;
		}
	}

	return e;
}

int HKDComponentClient::destroyClient()
{
	asioService.stopService();
	return worker ? worker->stopWorker() : eBadOperate;
}

void HKDComponentClient::afterPolledDataFromWorkerCallback(
	const std::string roleID, 
	const std::string flagID, 
	const std::string fromID, 
	const std::string toID, 
	const std::string data)
{
	DataPacketPtr pkt{ DataParser().parseData(data) };

	if (pkt)
	{
		MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
		const base::packet::MessagePacketType type{ msgpkt->getMessagePacketType() };

		if (base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_COMPONENT == type)
		{
			processComponentMessage(pkt);
		}
		else if (base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_DEVICE == type)
		{
			processDeviceMessage(fromID, toID, pkt);
		}
		else if (base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_EVENT == type)
		{
			processEventMessage(fromID, toID, pkt);
		}
		else
		{
			LOG(INFO) << "Parse HKD client polled message with unknown type = " << static_cast<int>(type) << ".";
		}
	}
	else
	{
		LOG(ERROR) << "Parse HKD client polled message failed.";
	}
}

void HKDComponentClient::sendRegisterWorkerServerMessage()
{
	std::string name, id;
	XMLParser().getValueByName("Config.xml", "Component.HKD.ID", id);
	XMLParser().getValueByName("Config.xml", "Component.HKD.Name", name);
	AbstractComponent component(
		base::component::ComponentType::COMPONENT_TYPE_HKD);
	component.setComponentID(id);
	component.setComponentName(name);

	DataPacketPtr pkt{
		boost::make_shared<MessagePacket>(
			base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_COMPONENT) };
	if (pkt)
	{
		MessagePacketPtr mp{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
		mp->setMessagePacketCommand(
			static_cast<int>(base::protocol::ComponentCommand::COMPONENT_COMMAND_SIGNIN_REQ));
		mp->setPacketData(&component);
		const std::string data{ DataPacker().packData(pkt) };
		sendData("worker", "request", id, parentXMQID, data);
	}
}

int HKDComponentClient::sendData(
	const std::string roleID, 
	const std::string flagID, 
	const std::string fromID, 
	const std::string toID, 
	const std::string data)
{
	return worker ? worker->sendData(roleID, flagID, fromID, toID, data) : eBadOperate;
}

const std::string HKDComponentClient::getHKDClientInfoByName(const std::string name) const
{
	std::string value;
	XMLParser().getValueByName("Config.xml", name, value);
	return value;
}

void HKDComponentClient::setHKDClientInfoWithName(
	const std::string name, const std::string value)
{
	XMLPacker().setValueWithName("Config.xml", name, value);
}

void HKDComponentClient::processComponentMessage(DataPacketPtr pkt)
{
	MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
	const base::protocol::ComponentCommand command{
		static_cast<base::protocol::ComponentCommand>(msgpkt->getMessagePacketCommand()) };

	if (base::protocol::ComponentCommand::COMPONENT_COMMAND_SIGNIN_REP == command)
	{
// 		const char* componentID{
// 			reinterpret_cast<const char*>(pkt->getPacketData()) };
// 		//无论注册还是心跳都保存组件ID标识
// 		setHKDClientInfoWithName("Component.HKD.ID", componentID);

		parentXMQID = reinterpret_cast<const char*>(pkt->getPacketData());
	}
}

void HKDComponentClient::processDeviceMessage(
	const std::string fromID, const std::string toID, DataPacketPtr pkt)
{
	MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
	const base::protocol::DeviceCommand command{
		static_cast<base::protocol::DeviceCommand>(msgpkt->getMessagePacketCommand()) };
	SurveillanceDevice* sd{ reinterpret_cast<SurveillanceDevice*>(pkt->getPacketData()) };
	int e{ eBadOperate };
	std::vector<AbstractCamera> cameras{};

	if (sd)
	{
		if (base::protocol::DeviceCommand::DEVICE_COMMAND_NEW_REQ == command)
		{
			e = addNewDevice(sd, cameras);
		}
		else if (base::protocol::DeviceCommand::DEVICE_COMMAND_DELETE_REQ == command)
		{
			e = deleteDeviceByID(sd->getDeviceID());
		}
		else if (base::protocol::DeviceCommand::DEVICE_COMMAND_MODIFY_REQ == command)
		{
			if (eSuccess == deleteDeviceByID(sd->getDeviceID()))
			{
				e = addNewDevice(sd, cameras);
			}
		}
	}

	replyMessageWithResult(
		fromID,
		toID,
		static_cast<int>(command) + 1,
		e,
		pkt->getPacketSequence() + 1,
		cameras,
		sd->getDeviceID());

	//设备登录/注销成功才进行流操作
	if (eSuccess == e)
	{
		e = processMediaStream((int)command, sd->getDeviceID(), cameras);
	}
}

void HKDComponentClient::processEventMessage(
	const std::string fromID, const std::string toID, DataPacketPtr pkt)
{
	MessagePacketPtr msgpkt{ 
		boost::dynamic_pointer_cast<MessagePacket>(pkt) };
	const base::protocol::EventCommand command{
		static_cast<base::protocol::EventCommand>(msgpkt->getMessagePacketCommand()) };
	const char* deviceID{ reinterpret_cast<const char*>(pkt->getPacketData()) };
	const char* cameraID{ reinterpret_cast<const char*>(pkt->getPacketData(1)) };
	const int* cameraIdx{ reinterpret_cast<const int*>(pkt->getPacketData(2)) };
	AbstractDevicePtr device{ deviceGroup.at(deviceID) };
	int e{ device ? eSuccess : eObjectNotExist };
	//默认抓图500KB以内的图片
	const int jpegBytes{ 500 * 1024 };
	char jpegData[jpegBytes]{ 0 };
	int jpegRet{ 0 };

	if (eSuccess == e)
	{
		SurveillanceDevicePtr sdp{ 
			boost::dynamic_pointer_cast<SurveillanceDevice>(device) };

		if (base::protocol::EventCommand::EVENT_COMMAND_CAPTURE_JPEG_REQ == command)
		{
			jpegRet = sdp->captureRealplayJPEGImage(*cameraIdx, jpegData, jpegBytes);
			LOG(INFO) << "Capture camera = " << *cameraIdx << " JPEG image size = " << jpegRet << ".";
		}
	}
	else
	{
		LOG(WARNING) << "Device ID is invalid.";
	}

	msgpkt->setMessagePacketCommand(static_cast<int>(command) + 1);
	msgpkt->setMessageStatus(0 < jpegRet ? eSuccess : eBadOperate);
	msgpkt->setPacketSequence(pkt->getPacketSequence() + 1);
	pkt->setPacketData(jpegData);
	const std::string rep{ DataPacker().packData(pkt) };

	//客户端应答时必须交换fromID和toID
	e = sendData("worker", "response", toID, fromID, rep);
}

int HKDComponentClient::processMediaStream(
	const int command, 
	const std::string did, 
	const std::vector<AbstractCamera> cameras)
{
	int e{ eSuccess };
	const base::protocol::DeviceCommand command_{ static_cast<base::protocol::DeviceCommand>(command) };

	if (base::protocol::DeviceCommand::DEVICE_COMMAND_DELETE_REQ == command_ ||
		base::protocol::DeviceCommand::DEVICE_COMMAND_MODIFY_REQ == command_)
	{
		destroySourceStreamSession(did);
	}

	for (int i = 0; i != cameras.size(); ++i)
	{
		createNewSourceStreamConnection(did, cameras[i].getCameraIndex());
	}

	return e;
}

int HKDComponentClient::addNewDevice(
	SurveillanceDevice* sd, 
	std::vector<AbstractCamera>& cameras)
{
	int e{ eBadNewObject };
	const std::string did{ sd->getDeviceID() };
	const base::device::SurveillanceDeviceType dt{ sd->getDeviceType() };
	const std::string address{ sd->getDeviceIPv4Address() }, name{ sd->getLoginUserName() }, password{ sd->getLoginUserPassword() };
	const unsigned short port{ sd->getDevicePortNumber() };

	AbstractDevicePtr adp{ deviceGroup.at(did) };
	if (!adp)
	{
		adp = boost::make_shared<HikvisionDevice>(did, dt);
		if (adp)
		{
			SurveillanceDevicePtr sdp{
				boost::dynamic_pointer_cast<SurveillanceDevice>(adp) };
			sdp->setDeviceIPv4Address(address);
			sdp->setDevicePortNumber(port);
			sdp->setLoginUserName(name);
			sdp->setLoginUserPassword(password);
			e = adp->startDevice();

			if (eSuccess == e)
			{
				sdp->getDeviceCamera(cameras);
				deviceGroup.insert(did, adp);
				LOG(INFO) << "Start new device successfully with address = " << address <<
					", port = " << port <<
					", name = " << name <<
					", password = " << password <<
					", ID = " << did << ".";
			}
			else
			{
				LOG(WARNING) << "Start new device failed with address = " << address <<
					", port = " << port <<
					", name = " << name <<
					", password = " << password <<
					", ID = " << did << ".";
			}
		}
		else
		{
			LOG(ERROR) << "Can not alloc memory for creating new device.";
		}
	} 
	else
	{
		e = eObjectExisted;
		LOG(WARNING) << "Start new device mutiple times with address = " << address <<
			", port = " << port <<
			", name = " << name <<
			", password = " << password <<
			", ID = " << did << ".";
	}

	return e;
}

int HKDComponentClient::deleteDeviceByID(const std::string did)
{
	int e{ eOutOfRange };
	AbstractDevicePtr device{ deviceGroup.at(did) };

	if (device)
	{
		e = device->stopDevice();
		if (eSuccess == e)
		{
			deviceGroup.remove(did);
		}
		
		LOG(INFO) << "Remove device ID = " << did << ".";
	}
	else
	{
		LOG(WARNING) << "Can not find device ID = " << did << ".";
	}

	return e;
}

int HKDComponentClient::replyMessageWithResult(
	const std::string fromID, 
	const std::string toID,
	const int command, 
	const int result, 
	const long long sequence, 
	const std::vector<AbstractCamera>& cameras,
	const std::string did)
{
	int e{ eBadNewObject };

	DataPacketPtr pkt{
		boost::make_shared<MessagePacket>(base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_DEVICE) };

	if (pkt)
	{
		MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
		msgpkt->setMessagePacketCommand(command);
		msgpkt->setMessageStatus(result);
		msgpkt->setPacketSequence(sequence);
		if (0 < cameras.size())
		{
			pkt->setPacketData((void*)&cameras);
		}
		else
		{
			pkt->setPacketData(nullptr);
		}
		pkt->setPacketData((void*)did.c_str());
		const std::string rep{ DataPacker().packData(pkt) };

		//客户端应答时必须交换fromID和toID
		e = sendData("worker", "response", toID, fromID, rep);
	}

	return e;
}

int HKDComponentClient::createNewSourceStreamConnection(
	const std::string did, 
	const int idx /* = -1 */)
{
	int e{ did.empty() || 0 > idx ? eInvalidParameter : eSuccess };

	if (eSuccess == e)
	{
		TCPConnectorPtr connector{ 
			boost::make_shared<TCPConnector>(
				asioService,
				boost::bind(&HKDComponentClient::afterRemoteConnectedNotificationCallback, this, _1, _2)) };
		if (connector)
		{
			urlGroup.pushBack((boost::format("%s:%d") % did % idx).str());
			e = connector->setConnect(mediaIP.c_str(), mediaPort);
		}
		else
		{
			e = eBadNewObject;
		}
	}

	return e;
}

int HKDComponentClient::createNewSourceStreamSession(
	const std::string url,
	boost::asio::ip::tcp::socket* s)
{
	int e{ url.empty() || !s ? eInvalidParameter : eSuccess };

	if (eSuccess == e)
	{
		std::vector<std::string> idGroup;
		boost::split(idGroup, url, boost::is_any_of(":"));
		AbstractDevicePtr device{ deviceGroup.at(idGroup[0]) };

		if (device)
		{
			boost::shared_ptr<HikvisionDevice> hkd{
				boost::dynamic_pointer_cast<HikvisionDevice>(device) };
			TCPSessionPtr session{
				boost::make_shared<HKDMediaStreamSession>(url, hkd->getUserID(), s) };

			if (session)
			{
				e = session->startSession();
				if (eSuccess == e)
				{
					streamSessionGroup.insert(url, session);
				}
				else
				{
					e = eBadOperate;
				}
			}
			else
			{
				e = eBadNewObject;
			}
		}
		else
		{
			e = eObjectNotExist;
		}
	}

	return e;
}

int HKDComponentClient::destroySourceStreamSession(const std::string did)
{
	int e{ did.empty() ? eInvalidParameter : eSuccess };

	if (eSuccess == e)
	{
		streamSessionGroup.removeHas(did);
		LOG(INFO) << "Destroy all of source stream session = " << did << " Successfully.";
	}

	return e;
}

void HKDComponentClient::afterRemoteConnectedNotificationCallback(
	boost::asio::ip::tcp::socket* s, 
	const boost::system::error_code e)
{
	if (s && !e)
	{
		const std::string url{ urlGroup.popFront() };
		if (!url.empty())
		{
			int e{ createNewSourceStreamSession(url, s) };

			if (eSuccess == e)
			{
				LOG(INFO) << "Create new source stream = " << url << " Successfully.";
			}
			else
			{
				LOG(WARNING) << "Create new source stream = " << url << " failed, result = " << e << ".";
			}
		}
	}
}
