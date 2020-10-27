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
#include "Protocol/EventPhrase.h"
#include "Xml/XmlCodec.h"
using XMLParser = base::xml::XMLParser;
using XMLPacker = base::xml::XMLPacker;
#include "Component/AbstractComponent.h"
using AbstractComponent = base::component::AbstractComponent;
#include "Packet/Message/MessagePacket.h"
using MessagePacket = base::packet::MessagePacket;
using MessagePacketPtr = boost::shared_ptr<MessagePacket>;
#include "AISPusherComponentClient.h"

AISPusherComponentClient::AISPusherComponentClient()
	: AbstractWorker()
{}
AISPusherComponentClient::~AISPusherComponentClient() {}

int AISPusherComponentClient::createNewClient(const std::string address)
{
	int e{ !address.empty() ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		MajordomoWorkerPtr mdwp{
			boost::make_shared<MajordomoWorker>(
				boost::bind(&AISPusherComponentClient::afterPolledDataFromWorkerCallback, this, _1, _2, _3, _4, _5)) };
		if (mdwp && eSuccess == mdwp->startWorker(address))
		{
			worker.swap(mdwp);
			//在客户端注册或心跳之前创建UUID标识
			AbstractWorker::generateUUIDWithName("AIS");
			e = AbstractWorker::createNewClient("");
		}
		else
		{
			e = eBadNewObject;
		}
	}

	return e;
}

int AISPusherComponentClient::destroyClient()
{
	return worker ? worker->stopWorker() : eBadOperate;
}

void AISPusherComponentClient::afterPolledDataFromWorkerCallback(
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
			LOG(INFO) << "Parse AIS client polled message with unknown type = " << static_cast<int>(type) << ".";
		}
	}
	else
	{
		LOG(ERROR) << "Parse AIS client polled message failed.";
	}
}

void AISPusherComponentClient::sendRegisterWorkerServerMessage()
{
	std::string name, id;
	XMLParser().getValueByName("Config.xml", "Component.AIS.ID", id);
	XMLParser().getValueByName("Config.xml", "Component.AIS.Name", name);
	AbstractComponent component(
		base::component::ComponentType::COMPONENT_TYPE_AIS);
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

void AISPusherComponentClient::sendQuerySystemServiceMessage()
{
	boost::shared_ptr<DataPacket> pkt{
		boost::make_shared<MessagePacket>(
			base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_COMPONENT) };

	if (pkt)
	{
		MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
		msgpkt->setMessagePacketCommand(
			static_cast<int>(base::protocol::ComponentCommand::COMPONENT_COMMAND_QUERY_REQ));
		const std::string data{ DataPacker().packData(pkt) };
		sendData("worker", "request", getUUID(), parentXMQID, data);
	}
}

int AISPusherComponentClient::sendData(
	const std::string roleID,
	const std::string flagID,
	const std::string fromID,
	const std::string toID,
	const std::string data)
{
	return worker ? worker->sendData(roleID, flagID, fromID, toID, data) : eBadOperate;
}

const std::string AISPusherComponentClient::getMediaStreamClientInfoByName(const std::string name) const
{
	std::string value;
	XMLParser().getValueByName("Config.xml", name, value);
	return value;
}

void AISPusherComponentClient::setMediaStreamClientInfoWithName(
	const std::string name, 
	const std::string value)
{
	XMLPacker().setValueWithName("Config.xml", name, value);
}

void AISPusherComponentClient::processComponentMessage(DataPacketPtr pkt)
{
	MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
	const base::protocol::ComponentCommand command{
		static_cast<base::protocol::ComponentCommand>(msgpkt->getMessagePacketCommand()) };

	if (base::protocol::ComponentCommand::COMPONENT_COMMAND_SIGNIN_REP == command)
	{
// 		const char* componentID{
// 			reinterpret_cast<const char*>(pkt->getPacketData()) };
// 		//无论注册还是心跳都保存组件ID标识
// 		setMediaStreamClientInfoWithName("Component.AIS.ID", componentID);

		parentXMQID = reinterpret_cast<const char*>(pkt->getPacketData());
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

void AISPusherComponentClient::buildAISInfoMessage(const int sailStatus /* = 0 */)
{
	if (!webComponentID.empty())
	{
		DataPacketPtr pkt{
		boost::make_shared<MessagePacket>(
			base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_EVENT) };
		if (pkt)
		{
			MessagePacketPtr mp{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
			mp->setMessagePacketCommand(
				static_cast<int>(base::protocol::EventCommand::EVENT_COMMAND_SYNC_AIS));
			mp->setPacketData((void*)&sailStatus);
			mp->setPacketData((void*)"yyyyy.yy,a");
			mp->setPacketData((void*)"llll.ll");
			const std::string data{ DataPacker().packData(pkt) };
			sendData("worker", "notification", getUUID(), webComponentID, data);
			LOG(INFO) << "Push AIS sail status = " << sailStatus << " to WEB service " << webComponentID << ".";
		}
	}
}
