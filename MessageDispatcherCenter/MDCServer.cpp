#include "boost/algorithm/string.hpp"
#include "boost/pointer_cast.hpp"
#include "boost/make_shared.hpp"
#include "boost/uuid/uuid.hpp"
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
#include "Time/XTime.h"
using base::time::Time;
#include "Protocol/DataPhrase.h"
using DataParser = base::protocol::DataParser;
using DataPacker = base::protocol::DataPacker;
#include "Protocol/ComponentPhrase.h"
//#include "Protocol/Device/DevicePhrase.h"
#include "Xml/XmlCodec.h"
using XMLParser = base::xml::XMLParser;
using XMLPacker = base::xml::XMLPacker;
#include "Packet/Message/MessagePacket.h"
using MessagePacket = base::packet::MessagePacket;
using MessagePacketPtr = boost::shared_ptr<MessagePacket>;
#include "Component/AbstractComponent.h"
using AbstractComponent = base::component::AbstractComponent;
using AbstractComponentPtr = boost::shared_ptr<AbstractComponent>;
#include "MDCServer.h"

MDCServer::MDCServer(
	const ServerModuleType server/* = ServerModuleType::SERVER_MODULE_TYPE_NONE*/,
	const ClientModuleType upstream/* = ClientModuleType::CLIENT_MODULE_TYPE_NONE*/,
	const std::string address/* = "tcp:\\127.0.0.1:61001"*/,
	const std::string name/* = "MDCServer"*/)
	: AbstractUpstreamServer(server, upstream, address)
{
	setMDCServerInfoWithName("Component.Disptcher.Name", name);
}

MDCServer::~MDCServer()
{}

void MDCServer::afterServerPolledMessageProcess(
	const std::string commID, 
	const std::string flagID, 
	const std::string fromID, 
	const std::string toID, 
	const std::string msg)
{
// 	LOG(INFO) << "Server polled message with CommID = " << commID
// 		<< ", FlagID = " << flagID
// 		<< ", fromID = " << fromID
// 		<< ", toID = " << toID << ".";

	//toID为空表示消息在MDC处理,
	//否则向toID的服务转发消息
	if (toID.empty())
	{
		processServerPolledMessage(commID, flagID, fromID, toID, msg);
	} 
	else
	{
		forwardServerPolledMessage(commID, flagID, fromID, toID, msg);
	}
}

void MDCServer::afterClientPolledMessageProcess(
	const std::string flagID, 
	const std::string fromID, 
	const std::string toID, 
	const std::string msg)
{
	//客户端消息处理
	DataPacketPtr pkt{ DataParser().parseData(msg) };

	if (pkt)
	{
		MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
		const base::packet::MessagePacketType type{ msgpkt->getMessagePacketType() };

		if (base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_COMPONENT == type)
		{
			processComponentMessage("", flagID, fromID, pkt);
		}
		else if (base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_COMPONENT == type ||
			base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_ALGORITHM == type ||
			base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_CREW == type || 
			base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_DEVICE == type ||
			base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_STATUS == type)
		{
			//直接查询WEB组件ID标识并向其转发消息
			std::vector<AbstractComponentPtr> components;
			registerComponentGroup.values(components);

			for (int i = 0; i != components.size(); ++i)
			{
				if (base::component::ComponentType::COMPONENT_TYPE_WEB == components[i]->getComponentType())
				{
					forwardServerPolledMessage(
						components[i]->getCommunicationID(), 
						flagID, 
						fromID, 
						components[i]->getComponentID(),
						msg);
					LOG(INFO) << "Forward upstream polled message to WEB service.";
				}
			}
		}
		else
		{
			LOG(INFO) << "Parse client polled message with unknown type = " << static_cast<int>(type) << ".";
		}
	}
	else
	{
		LOG(ERROR) << "Parse client polled message failed.";
	}
}

void MDCServer::afterAutoCheckConnectionTimeoutProcess()
{
	std::vector<AbstractComponentPtr> acp;
	registerComponentGroup.values(acp);
	//90s超时
	const long long timeout{ 90000 }, nowTime{ Time().tickcount() };

	for (int i = 0; i != acp.size(); ++i)
	{
		if (timeout < nowTime - acp[i]->getComponentTimestamp())
		{
			removeComponentByID(acp[i]->getComponentID());
		}
	}
}

const std::string MDCServer::buildAutoRegisterToBrokerMessage()
{
	std::string msgstr;
	AbstractComponent component(
		base::component::ComponentType::COMPONENT_TYPE_XMQ);
	component.setComponentID(getMDCServerInfoByName("Component.Disptcher.ID"));
	component.setComponentName(getMDCServerInfoByName("Component.Disptcher.Name"));
	DataPacketPtr datapkt{ 
		boost::make_shared<MessagePacket>(
			base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_COMPONENT) };

	if (datapkt)
	{
		MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(datapkt) };
		msgpkt->setMessagePacketCommand(
			static_cast<int>(base::protocol::ComponentCommand::COMPONENT_COMMAND_SIGNIN_REQ));
		datapkt->setPacketData(&component);
		msgstr = DataPacker().packData(datapkt);
	}

	return msgstr;
}

const std::string MDCServer::getMDCServerInfoByName(const std::string name) const
{
	std::string value;
	XMLParser().getValueByName("Config.xml", name, value);
	return value;
}

void MDCServer::setMDCServerInfoWithName(
	const std::string name, const std::string value)
{
	XMLPacker().setValueWithName("Config.xml", name, value);
}

void MDCServer::processServerPolledMessage(
	const std::string commID,
	const std::string flagID,
	const std::string fromID,
	const std::string toID,
	const std::string msg)
{
	//服务端只处理组件类消息
	DataPacketPtr pkt{ DataParser().parseData(msg) };

	if (pkt)
	{
		MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
		const base::packet::MessagePacketType type{ msgpkt->getMessagePacketType() };

		if (base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_COMPONENT == type)
		{
			processComponentMessage(commID, flagID, fromID, pkt);
		}
		else
		{
			LOG(INFO) << "Parse server polled message with unknown type = " << static_cast<int>(type) << ".";
		}
	}
	else
	{
		LOG(ERROR) << "Parse server polled message failed.";
	}
}

void MDCServer::forwardServerPolledMessage(
	const std::string commID,
	const std::string flagID,
	const std::string fromID,
	const std::string toID,
	const std::string msg)
{
	if (!flagID.compare("request") || !flagID.compare("notify"))
	{
		//分解toID查看下一个路由地址
		std::vector<std::string> addressGroup;
		boost::split(addressGroup, toID, boost::is_any_of(":"));
		AbstractComponentPtr component{ registerComponentGroup.at(addressGroup[0]) };

		if (component)
		{
			const std::size_t pos{ toID.find_first_of(":") };
			const std::string communicationID{ component->getCommunicationID() };
			int e{ 
				AbstractServer::sendMessageData(
					communicationID,
					flagID, 
					fromID, 
					toID.substr(-1 < pos ? pos : 0, toID.length()), 
					msg) };

			if (eSuccess == e)
			{
				LOG(INFO) << "Forward request / notify message to component name = " << component->getComponentName()
					<< ", CommunicationID = " << communicationID
// 					<< ", FlagID = " << flagID
// 					<< ", fromID = " << fromID
// 					<< ", toID = " << toID
// 					<< ", length = " << msg.length() 
					<< ".";
			} 
			else
			{
				LOG(WARNING) << "Forward request / notify message failed to component name = " << component->getComponentName()
					<< ", CommunicationID = " << communicationID
// 					<< ", FlagID = " << flagID
// 					<< ", fromID = " << fromID
// 					<< ", toID = " << toID
// 					<< ", length = " << msg.length() 
					<< ".";
			}
		} 
		else
		{
			LOG(ERROR) << "Can not forward to component ID = " << addressGroup[0] << ".";
		}
	}
	else if (!flagID.compare("response"))
	{
		int e{ eSuccess };
		std::string addressID{ "Upstream" };
		AbstractComponentPtr component{ registerComponentGroup.at(toID) };

		//如果toID存在则在服务端转发
		//如果toID不存在则在客户端转发
		if (component)
		{
			e = AbstractServer::sendMessageData(component->getCommunicationID(), flagID, fromID, toID, msg);
			addressID = component->getComponentName();
		} 
		else
		{
			//不要交换fromID和toID
			//发送端发送时交换,转发就不要交换了
			e = AbstractClient::sendMessageData(flagID, fromID, toID, msg);
		}

		if (eSuccess == e)
		{
			LOG(INFO) << "Forward response message to component name = " << addressID
// 				<< ", FlagID = " << flagID
// 				<< ", fromID = " << fromID
// 				<< ", toID = " << toID
// 				<< ", length = " << msg.length() 
				<< ".";
		}
		else
		{
			LOG(WARNING) << "Forward response message failed to component name = " << addressID
// 				<< ", FlagID = " << flagID
// 				<< ", fromID = " << fromID
// 				<< ", toID = " << toID
// 				<< ", length = " << msg.length() 
				<< ".";
		}
	}
}

void MDCServer::processComponentMessage(
	const std::string commID, 
	const std::string flagID, 
	const std::string fromID, 
	DataPacketPtr pkt)
{
	MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
	const base::protocol::ComponentCommand command{ 
		static_cast<base::protocol::ComponentCommand>(msgpkt->getMessagePacketCommand()) };
	AbstractComponent* ac{ reinterpret_cast<AbstractComponent*>(pkt->getPacketData()) };

	if (base::protocol::ComponentCommand::COMPONENT_COMMAND_SIGNIN_REQ == command)
	{
		//组件ID为空表示组件请求注册
		//组件ID不空表示组件请求心跳

		std::string componentID{ ac->getComponentID() };
		const std::string componentName{ ac->getComponentName() };
		const base::component::ComponentType componentType{ ac->getComponentType() };
		int e{ eSuccess };

		if (componentID.empty())
		{
			componentID = boost::uuids::to_string(boost::uuids::random_generator()());
			//为新注册的组件分配ID标识
			e = addNewRegisterComponent(componentID, componentName, commID, componentType);
		}
		else
		{
			e = updateRegisterCompnent(componentID, componentName, commID, componentType);
		}

		//commID和fromID一样
		replyMessageWithResultAndExtendID(
			commID,
			componentID,
			fromID,
			static_cast<int>(base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_COMPONENT),
			static_cast<int>(base::protocol::ComponentCommand::COMPONENT_COMMAND_SIGNIN_REP),
			e,
			pkt->getPacketSequence() + 1);
	}
	else if (base::protocol::ComponentCommand::COMPONENT_COMMAND_SIGNIN_REP == command)
	{
		const char* componentID{
			reinterpret_cast<const char*>(pkt->getPacketData()) };
		//无论注册还是心跳都保存组件ID标识
		setMDCServerInfoWithName("Component.Disptcher.ID", componentID);
	}
	else if (base::protocol::ComponentCommand::COMPONENT_COMMAND_SIGNOUT_REQ == command)
	{
		removeComponentByID(ac->getComponentID());
	}
	else if (base::protocol::ComponentCommand::COMPONENT_COMMAND_QUERY_REQ == command)
	{
		std::vector<AbstractComponentPtr> componentPtrs;
		registerComponentGroup.values(componentPtrs);
		DataPacketPtr datapkt{
			boost::make_shared<MessagePacket>(base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_COMPONENT) };

		if (pkt)
		{
			MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(datapkt) };
			msgpkt->setMessagePacketCommand(
				static_cast<int>(base::protocol::ComponentCommand::COMPONENT_COMMAND_QUERY_REP));
			datapkt->setPacketSequence(pkt->getPacketSequence() + 1);
			datapkt->setPacketTimestamp(Time().tickcount());
			datapkt->setPacketData(&componentPtrs);

			const std::string msgstr{ DataPacker().packData(datapkt) };
			if (!msgstr.empty())
			{
				//commID和fromID一样
				AbstractServer::sendMessageData(commID, "response", "", fromID, msgstr);
			}
		}
	}
}

int MDCServer::addNewRegisterComponent(
	const std::string componentID, 
	const std::string serviceName, 
	const std::string communicationID, 
	const base::component::ComponentType componentType /* = base::component::ComponentType::COMPONENT_TYPE_NONE */)
{
	int e{ eBadNewObject };
	AbstractComponentPtr acp{ 
		boost::make_shared<AbstractComponent>(componentType) };

	if (acp)
	{
		acp->setComponentID(componentID);
		acp->setComponentName(serviceName);
		acp->setCommunicationID(communicationID);
		registerComponentGroup.insert(componentID, acp);
		e = eSuccess;

		LOG(INFO) << "Add a new component name = " << serviceName <<
			", ID = " << componentID <<
			", CommID = " << communicationID <<
//			", Name = " << serviceName <<
			", Timestamp = " << acp->getComponentTimestamp() 
			<< ".";
	} 
	else
	{
		LOG(WARNING) << "Failed to add new component name = " << serviceName <<
			", ID = " << componentID <<
			", CommID = " << communicationID <<
			".";
	}

	return e;
}

int MDCServer::updateRegisterCompnent(
	const std::string componentID, 
	const std::string serviceName, 
	const std::string communicationID, 
	const base::component::ComponentType componentType /* = base::component::ComponentType::COMPONENT_TYPE_NONE */)
{
	int e{ !componentID.empty() && !serviceName.empty() ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		//匹配组件ID标识以更新组件实例的时间戳和通信ID标识
		//如果匹配到组件ID标识则新增组件

		AbstractComponentPtr acp{ registerComponentGroup.at(componentID) };

		if (acp)
		{
			acp->setCommunicationID(communicationID);
			acp->setComponentTimestamp(Time().tickcount());
			LOG(INFO) << "Update component name = " << acp->getComponentName() <<
				", ComponentID = " << componentID <<
				", CommunicationID = " << communicationID << ".";
		}
		else
		{
			e = addNewRegisterComponent(componentID, serviceName, communicationID, componentType);
		}
	}

	return e;
}

int MDCServer::replyMessageWithResultAndExtendID(
	const std::string commID, 
	const std::string extendID, 
	const std::string fromID,
	const int pkttype /* = 0 */, 
	const int command /* = 0 */, 
	const int result /* = 0 */, 
	const long long sequence /* = 0 */)
{
	int e{ eBadNewObject };

	DataPacketPtr datapkt{
		boost::make_shared<MessagePacket>(static_cast<base::packet::MessagePacketType>(pkttype)) };

	if (datapkt)
	{
		datapkt->setPacketSequence(sequence);
		datapkt->setPacketTimestamp(Time().tickcount());
		datapkt->setPacketData((void*)extendID.c_str());
		MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(datapkt) };
		msgpkt->setMessageStatus(result);
		msgpkt->setMessagePacketCommand(command);

		if (base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_COMPONENT == static_cast<base::packet::MessagePacketType>(pkttype))
		{
			e = AbstractServer::sendMessageData(commID, "response", "", fromID, DataPacker().packData(datapkt));
		}
	}

	return e;
}

int MDCServer::removeComponentByID(const std::string componentID)
{
	int e{ componentID.empty() ? eInvalidParameter : eSuccess };

	if (eSuccess == e)
	{
		AbstractComponentPtr acp{ registerComponentGroup.at(componentID) };

		if (acp)
		{
			registerComponentGroup.remove(componentID);

			LOG(WARNING) << "Remove component Name = " << acp->getComponentName() <<
				", ComponentID = " << acp->getComponentID() <<
				", CommunicationID = " << acp->getCommunicationID() <<
//				", Name = " << acp->getComponentName() <<
				", Timestamp = " << acp->getComponentTimestamp() <<
				"[ Expire = " << Time().tickcount() - acp->getComponentTimestamp() << " ].";
		}
		else
		{
			e = eObjectNotExist;
			LOG(WARNING) << "Can not find component with ID = " << componentID << ".";
		}
	}

	return e;
}
