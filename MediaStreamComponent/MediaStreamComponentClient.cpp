#include "boost/algorithm/string.hpp"
#include "boost/bind.hpp"
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

MediaStreamComponentClient::MediaStreamComponentClient() : AbstractWorker() {}
MediaStreamComponentClient::~MediaStreamComponentClient() {}

int MediaStreamComponentClient::createNewClient(const std::string address)
{
	int e{ !address.empty() ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		MajordomoWorkerPtr mdwp{
			boost::make_shared<MajordomoWorker>(
				boost::bind(&MediaStreamComponentClient::afterPolledDataFromWorkerCallback, this, _1, _2, _3, _4, _5)) };
		if (mdwp && eSuccess == mdwp->startWorker(address))
		{
			worker.swap(mdwp);
			//�ڿͻ���ע�������֮ǰ����UUID��ʶ
			AbstractWorker::generateUUIDWithName("MED");
			e = AbstractWorker::createNewClient("");
		}
		else
		{
			e = eBadNewObject;
		}
	}

	return e;
}

int MediaStreamComponentClient::destroyClient()
{
	return worker ? worker->stopWorker() : eBadOperate;
}

void MediaStreamComponentClient::afterPolledDataFromWorkerCallback(
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

void MediaStreamComponentClient::sendRegisterWorkerServerMessage()
{
	std::string name, id;
	XMLParser().getValueByName("Config.xml", "Component.MED.ID", id);
	XMLParser().getValueByName("Config.xml", "Component.MED.Name", name);
	AbstractComponent component(
		base::component::ComponentType::COMPONENT_TYPE_MED);
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
		sendData("worker", "request", id.c_str(), parentXMQID, data);
	}
}

int MediaStreamComponentClient::sendData(
	const std::string roleID, 
	const std::string flagID, 
	const std::string fromID, 
	const std::string toID, 
	const std::string data)
{
	return worker ? worker->sendData(roleID, flagID, fromID, toID, data) : eBadOperate;
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
// 		const char* componentID{
// 			reinterpret_cast<const char*>(pkt->getPacketData()) };
// 		//����ע�ỹ���������������ID��ʶ
// 		setMediaStreamClientInfoWithName("Component.Stream.ID", componentID);

		parentXMQID = reinterpret_cast<const char*>(pkt->getPacketData());
	}
}
