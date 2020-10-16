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
#include "MediaStreamComponentClient.h"

MediaStreamComponentClient::MediaStreamComponentClient()
	: AbstractClient(base::network::ClientModuleType::CLIENT_MODULE_TYPE_MAJORDOMO_WORKER)
{}
MediaStreamComponentClient::~MediaStreamComponentClient() {}

void MediaStreamComponentClient::afterClientPolledMessageProcess(
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
		else
		{
			LOG(INFO) << "Parse stream client polled message with unknown type = " << static_cast<int>(type) << ".";
		}
	}
	else
	{
		LOG(ERROR) << "Parse stream client polled message failed.";
	}
}

const std::string MediaStreamComponentClient::buildAutoRegisterToBrokerMessage()
{
	std::string msgstr;
	AbstractComponent component(
		base::component::ComponentType::COMPONENT_TYPE_MED);
	component.setComponentID(getMediaStreamClientInfoByName("Component.Stream.ID"));
	component.setComponentName(getMediaStreamClientInfoByName("Component.Stream.Name"));
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

const std::string MediaStreamComponentClient::getMediaStreamClientInfoByName(const std::string name) const
{
	std::string value;
	XMLParser().getValueByName("Config.xml", name, value);
	return value;
}

void MediaStreamComponentClient::setMediaStreamClientInfoWithName(
	const std::string name, 
	const std::string value)
{
	XMLPacker().setValueWithName("Config.xml", name, value);
}

void MediaStreamComponentClient::processComponentMessage(DataPacketPtr pkt)
{
	MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
	const base::protocol::ComponentCommand command{
		static_cast<base::protocol::ComponentCommand>(msgpkt->getMessagePacketCommand()) };
//	AbstractComponent* ac{ reinterpret_cast<AbstractComponent*>(pkt->getPacketData()) };

	if (base::protocol::ComponentCommand::COMPONENT_COMMAND_SIGNIN_REP == command)
	{
		const char* componentID{
			reinterpret_cast<const char*>(pkt->getPacketData()) };
		//无论注册还是心跳都保存组件ID标识
		setMediaStreamClientInfoWithName("Component.Stream.ID", componentID);
	}
}
