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
#include "Xml/XmlCodec.h"
using XMLParser = base::xml::XMLParser;
using XMLPacker = base::xml::XMLPacker;
#include "Component/AbstractComponent.h"
using AbstractComponent = base::component::AbstractComponent;
#include "Packet/Message/MessagePacket.h"
using MessagePacket = base::packet::MessagePacket;
using MessagePacketPtr = boost::shared_ptr<MessagePacket>;
#include "AlarmPusherComponentClient.h"

AlarmPusherComponentClient::AlarmPusherComponentClient()
	: AbstractClient(base::network::ClientModuleType::CLIENT_MODULE_TYPE_MAJORDOMO_WORKER)
{}
AlarmPusherComponentClient::~AlarmPusherComponentClient() {}

void AlarmPusherComponentClient::afterClientPolledMessageProcess(
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
		else if (base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_ALARM == type)
		{
			processAlarmMessage(msg);
		}
		else
		{
			LOG(INFO) << "Parse alarm pusher client polled message with unknown type = " << static_cast<int>(type) << ".";
		}
	}
	else
	{
		LOG(ERROR) << "Parse alarm pusher client polled message failed.";
	}
}

const std::string AlarmPusherComponentClient::buildAutoRegisterToBrokerMessage()
{
	std::string msgstr;
	AbstractComponent component(
		base::component::ComponentType::COMPONENT_TYPE_ALM);
	component.setComponentID(getMediaStreamClientInfoByName("Component.Alarm.ID"));
	component.setComponentName(getMediaStreamClientInfoByName("Component.Alarm.Name"));
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

const std::string AlarmPusherComponentClient::buildAutoQueryRegisterSubroutineMessage()
{
	std::string msg;
	boost::shared_ptr<DataPacket> pkt{
		boost::make_shared<MessagePacket>(base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_COMPONENT) };

	if (pkt)
	{
		MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
		msgpkt->setMessagePacketCommand(
			static_cast<int>(base::protocol::ComponentCommand::COMPONENT_COMMAND_QUERY_REQ));
		msg = DataPacker().packData(pkt);
	}

	return msg;
}

const std::string AlarmPusherComponentClient::getMediaStreamClientInfoByName(const std::string name) const
{
	std::string value;
	XMLParser().getValueByName("Config.xml", name, value);
	return value;
}

void AlarmPusherComponentClient::setMediaStreamClientInfoWithName(
	const std::string name, 
	const std::string value)
{
	XMLPacker().setValueWithName("Config.xml", name, value);
}

void AlarmPusherComponentClient::processComponentMessage(DataPacketPtr pkt)
{
	MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
	const base::protocol::ComponentCommand command{
		static_cast<base::protocol::ComponentCommand>(msgpkt->getMessagePacketCommand()) };

	if (base::protocol::ComponentCommand::COMPONENT_COMMAND_SIGNIN_REP == command)
	{
		const char* componentID{
			reinterpret_cast<const char*>(pkt->getPacketData()) };
		//无论注册还是心跳都保存组件ID标识
		setMediaStreamClientInfoWithName("Component.Alarm.ID", componentID);
	}
	else if (base::protocol::ComponentCommand::COMPONENT_COMMAND_QUERY_REP == command)
	{
		int idx{ 0 };

		while (1)
		{
			AbstractComponent* ac{
				reinterpret_cast<AbstractComponent*>(pkt->getPacketData(idx++)) };

			if (ac)
			{
				const base::component::ComponentType type{ ac->getComponentType() };
				if (base::component::ComponentType::COMPONENT_TYPE_WEB == type)
				{
					//保存WEB子服务组件ID标识
					webComponentID = ac->getComponentID();
				}
			}
			else
			{
				break;
			}
		}
	}
}

void AlarmPusherComponentClient::processAlarmMessage(const std::string msg)
{
	AbstractClient::sendMessageData("notify", "", webComponentID, msg);
}
