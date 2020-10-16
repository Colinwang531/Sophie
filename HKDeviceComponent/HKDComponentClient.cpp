#include "boost/algorithm/string.hpp"
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
	: AbstractMediaStreamClient(address, port)
{}
HKDComponentClient::~HKDComponentClient() {}

void HKDComponentClient::afterClientPolledMessageProcess(
	const std::string flagID,
	const std::string fromID,
	const std::string toID,
	const std::string msg)
{
	DataPacketPtr pkt{ DataParser().parseData(msg) };

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
			processDeviceMessage(fromID, pkt);
		}
		else if (base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_EVENT == type)
		{
			processEventMessage(fromID, pkt);
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

const std::string HKDComponentClient::buildAutoRegisterToBrokerMessage()
{
	std::string msgstr;
	AbstractComponent component(
		base::component::ComponentType::COMPONENT_TYPE_HKD);
	component.setComponentID(getHKDClientInfoByName("Component.HKD.ID"));
	component.setComponentName(getHKDClientInfoByName("Component.HKD.Name"));
	boost::shared_ptr<DataPacket> pkt{
		boost::make_shared<MessagePacket>(base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_COMPONENT) };

	if (pkt)
	{
		MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
		msgpkt->setMessagePacketCommand(
			static_cast<int>(base::protocol::ComponentCommand::COMPONENT_COMMAND_SIGNIN_REQ));
		pkt->setPacketData(&component);
		msgstr = DataPacker().packData(pkt);
	}

	return msgstr;
}

int HKDComponentClient::createNewMediaStreamSession(
	const std::string url, 
	boost::asio::ip::tcp::socket* s)
{
	int e{ eNotSupport };
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
		}
	}

	return e;
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
		const char* componentID{
			reinterpret_cast<const char*>(pkt->getPacketData()) };
		//无论注册还是心跳都保存组件ID标识
		setHKDClientInfoWithName("Component.HKD.ID", componentID);
	}
}

void HKDComponentClient::processDeviceMessage(const std::string fromID, DataPacketPtr pkt)
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

void HKDComponentClient::processEventMessage(const std::string fromID, DataPacketPtr pkt)
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
		}
	}

	msgpkt->setMessagePacketCommand(static_cast<int>(command) + 1);
	msgpkt->setMessageStatus(0 < jpegRet ? eSuccess : eBadOperate);
	msgpkt->setPacketSequence(pkt->getPacketSequence() + 1);
	pkt->setPacketData(jpegData);
	const std::string rep{ DataPacker().packData(pkt) };

	//客户端应答时必须交换fromID和toID
	e = AbstractClient::sendMessageData("response", "", fromID, rep);
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
		for (int i = 0; i != cameras.size(); ++i)
		{
			AbstractMediaStreamClient::disconnectMediaServer(did);
		}
	}

	for (int i = 0; i != cameras.size(); ++i)
	{
		AbstractMediaStreamClient::connectMediaServerWithID(did, cameras[i].getCameraID(), cameras[i].getCameraIndex());
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
		e = AbstractClient::sendMessageData("response", "", fromID, rep);
	}

	return e;
}
