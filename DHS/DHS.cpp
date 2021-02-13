#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/make_shared.hpp"
#include "boost/pointer_cast.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#ifdef _WINDOWS
#include "glog/log_severity.h"
#include "glog/logging.h"
#else
#include "glog/log_severity.h"
#include "glog/logging.h"
#endif//_WINDOWS
#include "Error.h"
#include "Define.h"
#include "Data/Buffer.h"
#include "Hardware/Cpu.h"
using Cpu = framework::hardware::Cpu;
#include "Thread/ThreadPool.h"
using ThreadPool = framework::thread::ThreadPool;
#include "Data/Protocol/ProtocolComponent.h"
using ProtocolComponent = framework::data::ProtocolComponent;
#include "Data/Protocol/ProtocolDevice.h"
using ProtocolDevice = framework::data::ProtocolDevice;
#include "Network/ZMQ/Msg_.h"
using Msg = framework::network::zeromq::Msg;
#include "Device/DeviceFactory.h"
using DeviceFactory = framework::device::DeviceFactory;
using CameraInfos = framework::device::CameraInfos;
#include "Device/Hikvision/HikvisionDevice.h"
using HikvisionDevice = framework::device::HikvisionDevice;
using HikvisionDevicePtr = boost::shared_ptr<HikvisionDevice>;
#include "Camera/Hikvision/HikvisionCamera.h"
using HikvisionCamera = framework::device::HikvisionCamera;
using HikvisionCameraPtr = boost::shared_ptr<HikvisionCamera>;
#include "Network/ASIO/Core/TCPListener.h"
using TCPListener = framework::network::asio::TCPListener;
using TCPListenerPtr = boost::shared_ptr<TCPListener>;
#include "Network/ASIO/Session/TCPTargetSession.h"
using TCPSession = framework::network::asio::TCPSession;
using TCPSessionPtr = boost::shared_ptr<TCPSession>;
using TCPTargetSession = framework::network::asio::TCPTargetSession;
#include "DHS.h"

DHS::DHS(
	const std::string name,
	const std::string id,
	const unsigned short port /* = 0 */)
	: XMQPeer(name, id), mediaPort{ port }
{}
DHS::~DHS()
{}

int DHS::startOD(
	const char* ipv4 /* = nullptr */,
	const unsigned short port /* = 61001 */)
{
	//开启TCP监听
	const int coreNumber{ Cpu().getCPUCoreNumber() };
	for (int i = 0; i != 1/*coreNumber*/; ++i)
	{
		TCPListenerPtr lp{
			boost::make_shared<TCPListener>(
				ASIO::getIdleIO(),
				mediaPort,
				boost::bind(&DHS::afterTCPAcceptResultCallback, this, _1, _2)) };
		if (lp)
		{
			lp->listen();
		}
	}

	return XMQPeer::startOD(ipv4, port);
}

int DHS::stopOD()
{
	int e{ XMQPeer::stopOD() };

	if (eSuccess == e)
	{
		LOG(INFO) << "Stop DHS service successfully.";
	}
	else
	{
		LOG(WARNING) << "Stop DHS service failed, result = " << e << ".";
	}

	return e;
}

// void DHS::sentdDataNotification(
// 	const boost::system::error_code e,
// 	const int bytes /* = 0 */)
// {}
// 
// void DHS::receivedDataNotification(
// 	const unsigned char* data /* = nullptr */,
// 	const int bytes /* = 0 */,
// 	const char* senderAddress /* = nullptr */,
// 	const unsigned short senderPort /* = 0 */)
// {
// 	if (data && 0 < bytes && senderAddress && 0 < senderPort)
// 	{
// 		LOG(INFO) << "DHS service received message from = [ " << senderAddress <<
// 			" : " << senderPort <<
// 			" ] with [ " << data << " ].";
// 
// 		//TODO
// 		//如何校验data数据就是IPv4地址呢?
// 		//获取到DHS自身的IPv4地址才向CMS注册
// 		std::string name, id;
// 		XMLParser().getValueByName("Config.xml", "Component.DHS.ID", id);
// 		XMLParser().getValueByName("Config.xml", "Component.DHS.Name", name);
// 		//第一次启动的DHS没有ID标识
// 		//没有ID标识就自己创建并保存
// 		if (id.empty())
// 		{
// 			id.append(boost::uuids::to_string(boost::uuids::random_generator()()));
// 			XMLPacker().setValueWithName("Config.xml", "Component.DHS.ID", id);
// 		}
// 
// 		AbstractComponent component(
// 			base::component::ComponentType::COMPONENT_TYPE_HKD);
// 		component.setComponentID(id);
// 		component.setComponentName(name);
// 
// 		DataPacketPtr pkt{
// 			boost::make_shared<MessagePacket>(
// 				base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_COMPONENT) };
// 		if (pkt)
// 		{
// 			MessagePacketPtr mp{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
// 			mp->setMessagePacketCommand(
// 				static_cast<int>(base::protocol::ComponentCommand::COMPONENT_COMMAND_SIGNIN_REQ));
// 			mp->setPacketData(&component);
// 			const std::string data{ DataPacker().packData(pkt) };
// 			sendData("worker", "request", id, parentXMQID, data);
// 		}
// 	}
// 	else
// 	{
// 		LOG(WARNING) << "DHS service received invalid data, data = " << data <<
// 			", bytes = " << bytes <<
// 			", sender address = " << senderAddress <<
// 			", sender port = " << senderPort << ".";
// 	}
// }

void DHS::afterParsePolledMessage(
	const std::string module, 
	const std::string from, 
	const std::string to, 
	const std::vector<std::string> routers, 
	const std::vector<std::string> messages)
{
	//消息内容的最后2个部分一定是command和message
	const std::size_t msgDataNumber{ messages.size() };
	const std::string sequence{ messages[msgDataNumber - 4] };
	const std::string timestamp{ messages[msgDataNumber - 3] };
	const std::string command{ messages[msgDataNumber - 2] };
	const std::string msg{ messages[msgDataNumber - 1] };

	//处理组件注册请求消息
	if (!command.compare(gComponentCommandName))
	{
		processComponentMessage(module, from, to, sequence, timestamp, msg);
	}
	//处理设备配置请求消息
	else if (!command.compare(gDeviceCommandName))
	{
		processDeviceMessage(module, from, to, routers, sequence, timestamp, msg);
	}
	else
	{
		LOG(WARNING) << "DHS polled unknown command type of message with from [ " << from <<
			" ] to [ " << to <<
			" ] command [ " << command << " ].";
	}
}

const std::string DHS::buildRegisterMessage()
{
	using ComponentMsg = framework::data::ComponentMsg;
	ComponentMsg c;
	c.command = 1;
	c.status = -1;
	using ComponentData = framework::data::ComponentData;
	ComponentData cd;
	cd.type = 5;
	cd.name = XMQPeer::getName();
	cd.nickname = XMQPeer::getName();
	cd.ipv4 = XMQPeer::getLocalIPv4();
	cd.id = XMQPeer::getID();
	c.cds.push_back(cd);

	return ProtocolComponent().pack(c);
}

void DHS::processComponentMessage(
	const std::string module, 
	const std::string from, 
	const std::string to, 
	const std::string sequence, 
	const std::string timestamp, 
	const std::string msg)
{
	using ComponentMsg = framework::data::ComponentMsg;
	ComponentMsg cm;
	int e{ ProtocolComponent().unpack(msg, cm) };

	if (eSuccess == e)
	{
		if (2 == cm.command)
		{
			//SIGNIN_REP

			LOG(INFO) << "DHS process component message with module [  " << module <<
				" ] from [ " << from <<
				" ] to [ " << to <<
				" ], sequence = " << sequence <<
				", timestamp = " << timestamp <<
				", status = " << cm.status << ".";
		}
	}
	else
	{
		LOG(WARNING) << "DHS process component message with module [  " << module <<
			" ] from [ " << from <<
			" ] to [ " << to <<
			" ], sequence = " << sequence <<
			", timestamp = " << timestamp <<
			" failed, result = " << e << ".";
	}
}

void DHS::processDeviceMessage(
	const std::string module,
	const std::string from,
	const std::string to,
	const std::vector<std::string> routers,
	const std::string sequence,
	const std::string timestamp,
	const std::string msg)
{
	using DeviceMsg = framework::data::DeviceMsg;
	DeviceMsg dm;
	int e{ ProtocolDevice().unpack(msg, dm) };

	if (eSuccess == e)
	{
		if (1 == dm.command)
		{
			//NEW_REQ
			DevicePtr dp{
				DeviceFactory().get_mutable_instance().createNewDevice(
					dm.dds[0].did, 
					static_cast<framework::device::FactoryType>(dm.dds[0].factory), 
					static_cast<framework::device::DeviceType>(dm.dds[0].type)) };

			if (dp)
			{
				e = DeviceFactory().get_mutable_instance().loginDevice(
					dp, dm.dds[0].username, dm.dds[0].userpwd, dm.dds[0].ipv4, dm.dds[0].port);

				if (eSuccess == e)
				{
					std::vector<CameraInfos> cinfos;
					DeviceFactory().get_mutable_instance().getCameraInfos(dp, cinfos);
					for (int i = 0; i != cinfos.size(); ++i)
					{
						using CameraData = framework::data::CameraData;
						CameraData cd;
						cd.enable = cinfos[i].enable;
						cd.type = cinfos[i].type;
						cd.index = cinfos[i].idx;
						cd.did = cinfos[i].id;
						cd.ipv4 = cinfos[i].ipv4;
						dm.dds[0].cds.push_back(cd);
					}

					devices.insert(dm.dds[0].did, dp);
					LOG(INFO) << "Add a new device [  " << dm.dds[0].did << " ] with " << cinfos.size() << " camera(s).";
				}
			}
			else
			{
				e = eBadNewObject;
			}
		}
		else if (3 == dm.command)
		{
			//DELETE_REQ

			DevicePtr device{ devices.at(dm.dds[0].did) };

			if (device)
			{
				DeviceFactory().get_mutable_instance().logoutDevice(device);
				devices.remove(dm.dds[0].did);
				LOG(WARNING) << "DHS remove device [  " << dm.dds[0].did << " ].";
			}
			else
			{
				LOG(WARNING) << "DHS can not find device [  " << dm.dds[0].did << " ].";
			}
		}
		else if (5 == dm.command)
		{
			//QUERY_REQ
		}

		LOG(INFO) << "DHS process device message with module [  " << module <<
			" ] from [ " << from <<
			" ] to [ " << to <<
			" ], sequence = " << sequence <<
			", timestamp = " << timestamp <<
			", command = " << dm.command <<
			", IP = " << dm.dds[0].ipv4 <<
			", port = " << dm.dds[0].port <<
			", user = " << dm.dds[0].username <<
			", password = " << dm.dds[0].userpwd << 
			", ID = " << dm.dds[0].did << ".";
	}
	else
	{
		LOG(WARNING) << "DHS process device message with module [  " << module <<
			" ] from [ " << from <<
			" ] to [ " << to <<
			" ], sequence = " << sequence <<
			", timestamp = " << timestamp <<
			" failed, result = " << e << ".";
	}

	dm.status = e;
	++dm.command;

	//-------------------------------------------------------------------------------------------
	//| "" | "worker" | "web" | "cms" | sender | "" | sequence | timestamp | "device" | message |
	//-------------------------------------------------------------------------------------------
	Msg msg_;
	msg_.addMessage(ProtocolDevice().pack(dm));
	msg_.addMessage(gDeviceCommandName);
	msg_.addMessage(timestamp);
	msg_.addMessage(sequence);
	msg_.addMessage(gEmptyData);
	//发送端ID标识
	msg_.addMessage(routers[0]);
	msg_.addMessage(to);
	msg_.addMessage(from);
	msg_.addMessage(module);
	msg_.addMessage(gEmptyData);

	if (eSuccess == OD::sendMsg(&msg_))
	{
		LOG(INFO) << "DHS send message of device successfully.";
	}
	else
	{
		LOG(WARNING) << "DHS send message of device failed.";
	}
}

// int HKDComponentClient::sendData(
// 	const std::string roleID, 
// 	const std::string flagID, 
// 	const std::string fromID, 
// 	const std::string toID, 
// 	const std::string data)
// {
// 	return worker ? worker->sendData(roleID, flagID, fromID, toID, data) : eBadOperate;
// }
// 
// const std::string HKDComponentClient::getHKDClientInfoByName(const std::string name) const
// {
// 	std::string value;
// 	XMLParser().getValueByName("Config.xml", name, value);
// 	return value;
// }
// 
// void HKDComponentClient::setHKDClientInfoWithName(
// 	const std::string name, const std::string value)
// {
// 	XMLPacker().setValueWithName("Config.xml", name, value);
// }
// 
// void HKDComponentClient::processComponentMessage(DataPacketPtr pkt)
// {
// 	MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
// 	const base::protocol::ComponentCommand command{
// 		static_cast<base::protocol::ComponentCommand>(msgpkt->getMessagePacketCommand()) };
// 
// 	if (base::protocol::ComponentCommand::COMPONENT_COMMAND_SIGNIN_REP == command)
// 	{
// // 		const char* componentID{
// // 			reinterpret_cast<const char*>(pkt->getPacketData()) };
// // 		//无论注册还是心跳都保存组件ID标识
// // 		setHKDClientInfoWithName("Component.HKD.ID", componentID);
// 
// 		parentXMQID = reinterpret_cast<const char*>(pkt->getPacketData());
// 	}
// }
// 
// void HKDComponentClient::processDeviceMessage(
// 	const std::string fromID, const std::string toID, DataPacketPtr pkt)
// {
// 	MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
// 	const base::protocol::DeviceCommand command{
// 		static_cast<base::protocol::DeviceCommand>(msgpkt->getMessagePacketCommand()) };
// 	SurveillanceDevice* sd{ reinterpret_cast<SurveillanceDevice*>(pkt->getPacketData()) };
// 	int e{ eBadOperate };
// 	std::vector<AbstractCamera> cameras{};
// 
// 	if (sd)
// 	{
// 		if (base::protocol::DeviceCommand::DEVICE_COMMAND_NEW_REQ == command)
// 		{
// 			e = addNewDevice(sd, cameras);
// 		}
// 		else if (base::protocol::DeviceCommand::DEVICE_COMMAND_DELETE_REQ == command)
// 		{
// 			e = deleteDeviceByID(sd->getDeviceID());
// 		}
// 		else if (base::protocol::DeviceCommand::DEVICE_COMMAND_MODIFY_REQ == command)
// 		{
// 			deleteDeviceByID(sd->getDeviceID());
// 			e = addNewDevice(sd, cameras);
// 		}
// 	}
// 
// 	replyMessageWithResult(
// 		fromID,
// 		toID,
// 		static_cast<int>(command) + 1,
// 		e,
// 		pkt->getPacketSequence() + 1,
// 		cameras,
// 		sd->getDeviceID());
// 
// 	//设备登录/注销成功才进行流操作
// 	if (eSuccess == e)
// 	{
// 		e = processMediaStream((int)command, sd->getDeviceID(), cameras);
// 	}
// }
// 
// void HKDComponentClient::processEventMessage(
// 	const std::string fromID, const std::string toID, DataPacketPtr pkt)
// {
// 	MessagePacketPtr msgpkt{ 
// 		boost::dynamic_pointer_cast<MessagePacket>(pkt) };
// 	const base::protocol::EventCommand command{
// 		static_cast<base::protocol::EventCommand>(msgpkt->getMessagePacketCommand()) };
// 	const char* deviceID{ reinterpret_cast<const char*>(pkt->getPacketData()) };
// 	const char* cameraID{ reinterpret_cast<const char*>(pkt->getPacketData(1)) };
// 	const int* cameraIdx{ reinterpret_cast<const int*>(pkt->getPacketData(2)) };
// 	AbstractDevicePtr device{ deviceGroup.at(deviceID) };
// 	int e{ device ? eSuccess : eObjectNotExist };
// 	//默认抓图500KB以内的图片
// 	const int jpegBytes{ 500 * 1024 };
// 	char jpegData[jpegBytes]{ 0 };
// 	int jpegRet{ 0 };
// 
// 	if (eSuccess == e)
// 	{
// 		SurveillanceDevicePtr sdp{ 
// 			boost::dynamic_pointer_cast<SurveillanceDevice>(device) };
// 
// 		if (base::protocol::EventCommand::EVENT_COMMAND_CAPTURE_JPEG_REQ == command)
// 		{
// 			jpegRet = sdp->captureRealplayJPEGImage(*cameraIdx, jpegData, jpegBytes);
// 			LOG(INFO) << "Capture camera = " << *cameraIdx << " JPEG image size = " << jpegRet << ".";
// 		}
// 	}
// 	else
// 	{
// 		LOG(WARNING) << "Device ID is invalid.";
// 	}
// 
// 	msgpkt->setMessagePacketCommand(static_cast<int>(command) + 1);
// 	msgpkt->setMessageStatus(0 < jpegRet ? eSuccess : eBadOperate);
// 	msgpkt->setPacketSequence(pkt->getPacketSequence() + 1);
// 	pkt->setPacketData(jpegData);
// 	const std::string rep{ DataPacker().packData(pkt) };
// 
// 	//客户端应答时必须交换fromID和toID
// 	e = sendData("worker", "response", toID, fromID, rep);
// }
// 
// int HKDComponentClient::processMediaStream(
// 	const int command, 
// 	const std::string did, 
// 	const std::vector<AbstractCamera> cameras)
// {
// 	int e{ eSuccess };
// 	const base::protocol::DeviceCommand command_{ static_cast<base::protocol::DeviceCommand>(command) };
// 
// 	if (base::protocol::DeviceCommand::DEVICE_COMMAND_DELETE_REQ == command_ ||
// 		base::protocol::DeviceCommand::DEVICE_COMMAND_MODIFY_REQ == command_)
// 	{
// 		destroySourceStreamSession(did);
// 	}
// 
// 	for (int i = 0; i != cameras.size(); ++i)
// 	{
// 		createNewSourceStreamConnection(did, cameras[i].getCameraIndex());
// 	}
// 
// 	return e;
// }
// 
// int HKDComponentClient::addNewDevice(
// 	SurveillanceDevice* sd, 
// 	std::vector<AbstractCamera>& cameras)
// {
// 	int e{ eBadNewObject };
// 	const std::string did{ sd->getDeviceID() };
// 	const base::device::SurveillanceDeviceType dt{ sd->getDeviceType() };
// 	const std::string address{ sd->getDeviceIPv4Address() }, name{ sd->getLoginUserName() }, password{ sd->getLoginUserPassword() };
// 	const unsigned short port{ sd->getDevicePortNumber() };
// 
// 	AbstractDevicePtr adp{ deviceGroup.at(did) };
// 	if (!adp)
// 	{
// 		adp = boost::make_shared<HikvisionDevice>(did, dt);
// 		if (adp)
// 		{
// 			SurveillanceDevicePtr sdp{
// 				boost::dynamic_pointer_cast<SurveillanceDevice>(adp) };
// 			sdp->setDeviceIPv4Address(address);
// 			sdp->setDevicePortNumber(port);
// 			sdp->setLoginUserName(name);
// 			sdp->setLoginUserPassword(password);
// 			e = adp->startDevice();
// 
// 			if (eSuccess == e)
// 			{
// 				sdp->getDeviceCamera(cameras);
// 				deviceGroup.insert(did, adp);
// 				LOG(INFO) << "Start new device successfully with address = " << address <<
// 					", port = " << port <<
// 					", name = " << name <<
// 					", password = " << password <<
// 					", ID = " << did << ".";
// 			}
// 			else
// 			{
// 				LOG(WARNING) << "Start new device failed with address = " << address <<
// 					", port = " << port <<
// 					", name = " << name <<
// 					", password = " << password <<
// 					", ID = " << did << ".";
// 			}
// 		}
// 		else
// 		{
// 			LOG(ERROR) << "Can not alloc memory for creating new device.";
// 		}
// 	} 
// 	else
// 	{
// 		e = eObjectExisted;
// 		LOG(WARNING) << "Start new device mutiple times with address = " << address <<
// 			", port = " << port <<
// 			", name = " << name <<
// 			", password = " << password <<
// 			", ID = " << did << ".";
// 	}
// 
// 	return e;
// }
// 
// int HKDComponentClient::deleteDeviceByID(const std::string did)
// {
// 	int e{ eOutOfRange };
// 	AbstractDevicePtr device{ deviceGroup.at(did) };
// 
// 	if (device)
// 	{
// 		e = device->stopDevice();
// 		if (eSuccess == e)
// 		{
// 			deviceGroup.remove(did);
// 		}
// 		
// 		LOG(INFO) << "Remove device ID = " << did << ".";
// 	}
// 	else
// 	{
// 		LOG(WARNING) << "Can not find device ID = " << did << ".";
// 	}
// 
// 	return e;
// }
// 
// int HKDComponentClient::replyMessageWithResult(
// 	const std::string fromID, 
// 	const std::string toID,
// 	const int command, 
// 	const int result, 
// 	const long long sequence, 
// 	const std::vector<AbstractCamera>& cameras,
// 	const std::string did)
// {
// 	int e{ eBadNewObject };
// 
// 	DataPacketPtr pkt{
// 		boost::make_shared<MessagePacket>(base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_DEVICE) };
// 
// 	if (pkt)
// 	{
// 		MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
// 		msgpkt->setMessagePacketCommand(command);
// 		msgpkt->setMessageStatus(result);
// 		msgpkt->setPacketSequence(sequence);
// 		if (0 < cameras.size())
// 		{
// 			pkt->setPacketData((void*)&cameras);
// 		}
// 		else
// 		{
// 			pkt->setPacketData(nullptr);
// 		}
// 		pkt->setPacketData((void*)did.c_str());
// 		const std::string rep{ DataPacker().packData(pkt) };
// 
// 		//客户端应答时必须交换fromID和toID
// 		e = sendData("worker", "response", toID, fromID, rep);
// 	}
// 
// 	return e;
// }
// 
// int HKDComponentClient::createNewSourceStreamConnection(
// 	const std::string did, 
// 	const int idx /* = -1 */)
// {
// 	int e{ did.empty() || 0 > idx ? eInvalidParameter : eSuccess };
// 
// 	if (eSuccess == e)
// 	{
// 		TCPConnectorPtr connector{ 
// 			boost::make_shared<TCPConnector>(
// 				asioService,
// 				boost::bind(&HKDComponentClient::afterRemoteConnectedNotificationCallback, this, _1, _2)) };
// 		if (connector)
// 		{
// 			urlGroup.pushBack((boost::format("%s:%d") % did % idx).str());
// 			e = connector->setConnect(mediaIP.c_str(), mediaPort);
// 		}
// 		else
// 		{
// 			e = eBadNewObject;
// 		}
// 	}
// 
// 	return e;
// }
// 
// int HKDComponentClient::createNewSourceStreamSession(
// 	const std::string url,
// 	boost::asio::ip::tcp::socket* s)
// {
// 	int e{ url.empty() || !s ? eInvalidParameter : eSuccess };
// 
// 	if (eSuccess == e)
// 	{
// 		std::vector<std::string> idGroup;
// 		boost::split(idGroup, url, boost::is_any_of(":"));
// 		AbstractDevicePtr device{ deviceGroup.at(idGroup[0]) };
// 
// 		if (device)
// 		{
// 			boost::shared_ptr<HikvisionDevice> hkd{
// 				boost::dynamic_pointer_cast<HikvisionDevice>(device) };
// 			TCPSessionPtr session{
// 				boost::make_shared<HKDMediaStreamSession>(url, hkd->getUserID(), s) };
// 
// 			if (session)
// 			{
// 				e = session->startSession();
// 				if (eSuccess == e)
// 				{
// 					streamSessionGroup.insert(url, session);
// 				}
// 				else
// 				{
// 					e = eBadOperate;
// 				}
// 			}
// 			else
// 			{
// 				e = eBadNewObject;
// 			}
// 		}
// 		else
// 		{
// 			e = eObjectNotExist;
// 		}
// 	}
// 
// 	return e;
// }
// 
// int HKDComponentClient::destroySourceStreamSession(const std::string did)
// {
// 	int e{ did.empty() ? eInvalidParameter : eSuccess };
// 
// 	if (eSuccess == e)
// 	{
// 		streamSessionGroup.removeHas(did);
// 		urlGroup.clear();
// 		LOG(INFO) << "Destroy all of source stream session = " << did << " Successfully.";
// 	}
// 
// 	return e;
// }
// 
// void HKDComponentClient::afterRemoteConnectedNotificationCallback(
// 	boost::asio::ip::tcp::socket* s, 
// 	const boost::system::error_code e)
// {
// 	if (s && !e)
// 	{
// 		const std::string url{ urlGroup.popFront() };
// 		if (!url.empty())
// 		{
// 			int e{ createNewSourceStreamSession(url, s) };
// 
// 			if (eSuccess == e)
// 			{
// 				LOG(INFO) << "Create new source stream = " << url << " Successfully.";
// 			}
// 			else
// 			{
// 				LOG(WARNING) << "Create new source stream = " << url << " failed, result = " << e << ".";
// 			}
// 		}
// 		else
// 		{
// 			LOG(WARNING) << "Get empty url of stream, count = " << urlGroup.size() << ".";
// 		}
// 	}
// 	else
// 	{
// 		LOG(WARNING) << "Can not connect to stream, result = " << e.value() << ".";
// 	}
// }

void DHS::afterTCPAcceptResultCallback(
	const int e, 
	boost::asio::ip::tcp::socket& so)
{
	if (!e)
	{
		TCPSessionPtr tp{
			boost::make_shared<TCPTargetSession>(
				so,
				boost::bind(&DHS::afterTCPTargetReceiveResultCallback, this, _1, _2, _3)) };

		if (tp)
		{
			tp->receive();
		}
	}
}

void DHS::afterTCPTargetReceiveResultCallback(
	const char* data /* = nullptr */, 
	const int bytes /* = 0 */, 
	TCPSessionPtr tsp /* = nullptr */)
{
	if (data && bytes)
	{
		//检查每一帧头数据是否正确
		const int* flag{ (const int*)data };
		if (framework::data::FixedHeadFlag != *flag)
		{
			return;
		}

		const int* dataType{ (const int*)(data + 4) };
		const int* streamType{ (const int*)(data + 8) };
		const int* frameType{ (const int*)(data + 12) };
		const int* frameSize{ (const int*)(data + 16) };
		const long long* frameSequence{ (const long long*)(data + 20) };
		const long long* frameTimestamp{ (const long long*)(data + 28) };
		const std::string streamURL((const char*)(data + framework::data::FixedHeadSize), *frameSize);
		
		//打开视频流
		const size_t pos{ streamURL.find_first_of(':') };
		const std::string deviceID{ streamURL.substr(0, pos) };
		const int channelNumber{ atoi(streamURL.substr(pos, streamURL.length()).c_str()) };

		DevicePtr dp{ devices.at(deviceID) };

		if (dp)
		{
			HikvisionDevicePtr hkdp{ 
				boost::dynamic_pointer_cast<HikvisionDevice>(dp) };
			HikvisionCameraPtr hkc{ 
				boost::dynamic_pointer_cast<HikvisionCamera>(hkdp->getCamera(channelNumber)) };

			if (hkc)
			{
				hkc->openStream(tsp, hkdp->getLoginID());
			}
		}
	}
}
