//#include "boost/algorithm/string.hpp"
// #include "boost/bind.hpp"
#include "boost/format.hpp"
// #include "boost/make_shared.hpp"
// #include "boost/pointer_cast.hpp"
#ifdef _WINDOWS
#include "glog/log_severity.h"
#include "glog/logging.h"
#else
#include "glog/log_severity.h"
#include "glog/logging.h"
#endif//_WINDOWS
#include "Error.h"
#include "Define.h"
#include "Time/XTime.h"
using Time = framework::time::Time;
#include "Thread/ThreadPool.h"
using ThreadPool = framework::thread::ThreadPool;
#include "Data/Protocol/ProtocolComponent.h"
using ProtocolComponent = framework::data::ProtocolComponent;
#include "Data/Protocol/ProtocolClock.h"
using ProtocolClock = framework::data::ProtocolClock;
#include "Data/Protocol/ProtocolAlarm.h"
using ProtocolAlarm = framework::data::ProtocolAlarm;
#include "Network/ZMQ/Msg_.h"
using Msg = framework::network::zeromq::Msg;
#include "ALM.h"

ALM::ALM(
	const std::string name,
	const std::string id,
	const unsigned short port /* = 60820 */)
	: XMQPeer(name, id), Pub(), pubPort{ port }
{}
ALM::~ALM() {}

int ALM::startOD(
	const char* ipv4 /* = nullptr */, 
	const unsigned short port /* = 0 */)
{
	LOG(INFO) << "Starting ALM with parameters of command line [ name = " << XMQPeer::getName() <<
		", remote XMQ IPv4 = " << ipv4 <<
		", remote XMQ port = " << port << " ].";

	int e{ XMQPeer::startOD(ipv4, port) };

	if (eSuccess == e)
	{
		LOG(INFO) << "Starting ALM service successfully.";
		e = Pub::startPub(pubPort);
	}
	else
	{
		LOG(WARNING) << "Starting ALM service failed, result = " << e << ".";
	}

	return e;
}

int ALM::stopOD()
{
	int e{ XMQPeer::stopOD() };

	if (eSuccess == e)
	{
		e = Pub::stopPub();
		LOG(INFO) << "Stopping ALM service successfully.";
	}
	else
	{
		LOG(WARNING) << "Stopping ALM service failed, result = " << e << ".";
	}

	return e;
}

void ALM::afterParsePolledMessage(
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
	const std::string message{ messages[msgDataNumber - 1] };

	//处理组件注册请求消息
	if (!command.compare(gComponentCommandName))
	{
		processComponentMessage(module, from, to, sequence, timestamp, message);
	}
	//处理组件注册请求消息
	else if (!command.compare(gClockCommandName))
	{
		processClockMessage(module, from, to, sequence, timestamp, message);
	}
	//处理报警注册请求消息
	else if (!command.compare(gAlarmCommandName))
	{
		processAlarmMessage(module, from, to, sequence, timestamp, message);
	}
}

void ALM::processComponentMessage(
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

			LOG(INFO) << "ALM process component message with module [  " << module <<
				" ] from [ " << from <<
				" ] to [ " << to <<
				" ], sequence = " << sequence <<
				", timestamp = " << timestamp <<
				", status = " << cm.status << ".";
		}
	}
	else
	{
		LOG(WARNING) << "ALM process component message with module [  " << module <<
			" ] from [ " << from <<
			" ] to [ " << to <<
			" ], sequence = " << sequence <<
			", timestamp = " << timestamp <<
			" failed, result = " << e << ".";
	}
}

void ALM::processClockMessage(
	const std::string module,
	const std::string from,
	const std::string to,
	const std::string sequence,
	const std::string timestamp,
	const std::string msg)
{
	clock = ProtocolClock().unpack(msg);

// 	if (eSuccess == e)
// 	{
// 		if (2 == cm.command)
// 		{
// 			//SIGNIN_REP
// 
// 			LOG(INFO) << "ALM process component message with module [  " << module <<
// 				" ] from [ " << from <<
// 				" ] to [ " << to <<
// 				" ], sequence = " << sequence <<
// 				", timestamp = " << timestamp <<
// 				", status = " << cm.status << ".";
// 		}
// 	}
// 	else
// 	{
// 		LOG(WARNING) << "ALM process component message with module [  " << module <<
// 			" ] from [ " << from <<
// 			" ] to [ " << to <<
// 			" ], sequence = " << sequence <<
// 			", timestamp = " << timestamp <<
// 			" failed, result = " << e << ".";
// 	}
}

void ALM::processAlarmMessage(
	const std::string module, 
	const std::string from, 
	const std::string to, 
	const std::string sequence, 
	const std::string timestamp, 
	const std::string msg)
{
	using AlarmMsg = framework::data::AlarmMsg;
	AlarmMsg am;
	int e{ ProtocolAlarm().unpack(msg, am) };

	if (eSuccess == e)
	{
		am.alarm.timestamp = clock;

		//----------------------------------------------------------------------------------------------
		//| "" | "worker" | "alm" | "web" | sender | "" | sequence | timestamp | "algorithm" | message |
		//----------------------------------------------------------------------------------------------
		Msg msg_;
		msg_.addMessage(ProtocolAlarm().pack(am));
		msg_.addMessage(gAlarmCommandName);
		msg_.addMessage(timestamp);
		msg_.addMessage(sequence);
		msg_.addMessage(gEmptyData);
		msg_.addMessage(gALMComponentName);
		msg_.addMessage(gALMComponentName);
		msg_.addMessage(gWorkerModuleName);
		msg_.addMessage(gEmptyData);

		if (eSuccess == Pub::sendMsg(&msg_))
		{
			LOG(INFO) << "Alarm publish alarm message successfully.";
		}
		else
		{
			LOG(WARNING) << "Alarm publish alarm message failed.";
		}
	}
}

const std::string ALM::buildRegisterMessage()
{
	using ComponentMsg = framework::data::ComponentMsg;
	ComponentMsg c;
	c.command = 1;
	c.status = -1;
	using ComponentData = framework::data::ComponentData;
	ComponentData cd;
	cd.type = 17;
	cd.name = XMQPeer::getName();
	cd.nickname = XMQPeer::getName();
	cd.id = XMQPeer::getID();
	c.cds.push_back(cd);

	return ProtocolComponent().pack(c);
}

// void AlarmPusherComponentClient::sendRegisterWorkerServerMessage()
// {
// 	std::string name, id;
// 	XMLParser().getValueByName("Config.xml", "Component.ALM.ID", id);
// 	XMLParser().getValueByName("Config.xml", "Component.ALM.Name", name);
// 	AbstractComponent component(
// 		base::component::ComponentType::COMPONENT_TYPE_ALM);
// 	component.setComponentID(id);
// 	component.setComponentName(name);
// 
// 	DataPacketPtr pkt{
// 		boost::make_shared<MessagePacket>(
// 			base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_COMPONENT) };
// 	if (pkt)
// 	{
// 		MessagePacketPtr mp{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
// 		mp->setMessagePacketCommand(
// 			static_cast<int>(base::protocol::ComponentCommand::COMPONENT_COMMAND_SIGNIN_REQ));
// 		mp->setPacketData(&component);
// 		const std::string data{ DataPacker().packData(pkt) };
// 		sendData("worker", "request", id, parentXMQID, data);
// 	}
// }
// 
// void AlarmPusherComponentClient::sendQuerySystemServiceMessage()
// {
// 	boost::shared_ptr<DataPacket> pkt{
// 		boost::make_shared<MessagePacket>(
// 			base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_COMPONENT) };
// 
// 	if (pkt)
// 	{
// 		MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
// 		msgpkt->setMessagePacketCommand(
// 			static_cast<int>(base::protocol::ComponentCommand::COMPONENT_COMMAND_QUERY_REQ));
// 		const std::string data{ DataPacker().packData(pkt) };
// 		sendData("worker", "request", AbstractWorker::getUUID(), parentXMQID, data);
// 	}
// }
// 
// int AlarmPusherComponentClient::sendData(
// 	const std::string roleID, 
// 	const std::string flagID, 
// 	const std::string fromID, 
// 	const std::string toID, 
// 	const std::string data)
// {
// 	return worker ? worker->sendData(roleID, flagID, fromID, toID, data) : eBadOperate;
// }
// 
// const std::string AlarmPusherComponentClient::getMediaStreamClientInfoByName(const std::string name) const
// {
// 	std::string value;
// 	XMLParser().getValueByName("Config.xml", name, value);
// 	return value;
// }
// 
// void AlarmPusherComponentClient::setMediaStreamClientInfoWithName(
// 	const std::string name, 
// 	const std::string value)
// {
// 	XMLPacker().setValueWithName("Config.xml", name, value);
// }
// 
// void AlarmPusherComponentClient::processComponentMessage(DataPacketPtr pkt)
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
// // 		setMediaStreamClientInfoWithName("Component.Alarm.ID", componentID);
// 
// 		parentXMQID = reinterpret_cast<const char*>(pkt->getPacketData());
// 	}
// 	else if (base::protocol::ComponentCommand::COMPONENT_COMMAND_QUERY_REP == command)
// 	{
// 		int idx{ 0 };
// 
// 		while (1)
// 		{
// 			AbstractComponent* ac{
// 				reinterpret_cast<AbstractComponent*>(pkt->getPacketData(idx++)) };
// 
// 			if (ac)
// 			{
// 				const base::component::ComponentType type{ ac->getComponentType() };
// 				if (base::component::ComponentType::COMPONENT_TYPE_WEB == type)
// 				{
// 					//保存WEB子服务组件ID标识
// 					webComponentID = ac->getComponentID();
// 				}
// 			}
// 			else
// 			{
// 				break;
// 			}
// 		}
// 	}
// }
// 
// void AlarmPusherComponentClient::processAlarmMessage(DataPacketPtr pkt)
// {
// 	AlarmPacketPtr alarmpkt{ 
// 		boost::dynamic_pointer_cast<AlarmPacket>(pkt) };
// 	const base::protocol::AlarmType type{
// 		static_cast<base::protocol::AlarmType>(alarmpkt->getMessagePacketCommand()) };
// 	//追加母钟时间
// //	const std::string combine{ (boost::format("%s#%s") % msg % motherClockTime).str() };
// 	alarmpkt->setAlarmClock(motherClockTime.empty() ? "1970-01-01,00:00:00,+0" : motherClockTime.c_str());
// 	sendData("worker", "notification", AbstractWorker::getUUID(), webComponentID, DataPacker().packData(alarmpkt));
// 	LOG(INFO) << "Push alarm message type = " << (int)type/*AbstractWorker::getUUID()*/ << " [ " << motherClockTime << " ]," << " to WEB service " << webComponentID << ".";
// }
// 
// void AlarmPusherComponentClient::processEventMessage(DataPacketPtr pkt)
// {
// 	MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
// 	const base::protocol::EventCommand command{
// 		static_cast<base::protocol::EventCommand>(msgpkt->getMessagePacketCommand()) };
// 
// 	if (base::protocol::EventCommand::EVENT_COMMAND_SYNC_CLOCK == command)
// 	{
// 		motherClockTime = reinterpret_cast<const char*>(pkt->getPacketData());
// 	}
// }