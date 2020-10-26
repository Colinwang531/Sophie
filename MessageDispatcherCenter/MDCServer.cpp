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
#include "Xml/XmlCodec.h"
using XMLParser = base::xml::XMLParser;
using XMLPacker = base::xml::XMLPacker;
#include "Packet/Message/MessagePacket.h"
using MessagePacket = base::packet::MessagePacket;
using MessagePacketPtr = boost::shared_ptr<MessagePacket>;
#include "MDCServer.h"

MDCServer::MDCServer() : MajordomoUpstreamBroker()
{}

MDCServer::~MDCServer()
{}

void MDCServer::afterPolledDataFromServerCallback(
	const std::string commID, 
	const std::string roleID, 
	const std::string flagID, 
	const std::string fromID, 
	const std::string toID, 
	const std::string data)
{
	//toIDΪ�ջ��ߵ����Լ���ID��ʶ��ʾ��Ϣ��MDC����,
	//������toID�ķ���ת����Ϣ
	if (toID.empty() || !toID.compare(AbstractWorker::getUUID()))
	{
		if (!flagID.compare("upload"))
		{
			//�����ϼ�XMQת����Ϣ
			//�����Ϣת��ʧ���򽻸�WEB����
			int e{ sendData("worker", flagID, ownerXMQID, fromID, data) };

			if (eSuccess == e)
			{
				LOG(INFO) << "Forward upload message to upstream server.";
			}
			else
			{
				LOG(WARNING) << "Forward upload message to upstream service failed.";
			}
		}
		else
		{
			processPolledDataFromServer(commID, roleID, flagID, fromID, toID, data);
		}
	} 
	else
	{
		forwardPolledDataFromServer(commID, flagID, fromID, toID, data);
	}
}

void MDCServer::afterPolledDataFromWorkerCallback(
	const std::string roleID, 
	const std::string flagID, 
	const std::string fromID, 
	const std::string toID, 
	const std::string data)
{
	//�ͻ�����Ϣ����
	DataPacketPtr pkt{ DataParser().parseData(data) };

	if (pkt)
	{
		MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
		const base::packet::MessagePacketType type{ msgpkt->getMessagePacketType() };

		if (base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_COMPONENT == type)
		{
			processComponentMessage("", flagID, fromID, pkt);
		}
		else if (base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_ALGORITHM == type ||
			base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_CREW == type || 
			base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_DEVICE == type ||
			base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_STATUS == type)
		{
			//ȥ��toID��ʶ�еĵ�һ��·�ɵ�ַ
			std::vector<std::string> addressGroup;
			boost::split(addressGroup, toID, boost::is_any_of(":"));

			//ֱ�Ӳ�ѯWEB���ID��ʶ������ת����Ϣ
			std::vector<AbstractComponentPtr> components;
			registerComponentGroup.values(components);

			for (int i = 0; i != components.size(); ++i)
			{
				if (base::component::ComponentType::COMPONENT_TYPE_WEB == components[i]->getComponentType())
				{
					forwardPolledDataFromServer(
						components[i]->getCommunicationID(), 
						flagID, 
						fromID, 
						addressGroup[1],
						data);
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

void MDCServer::afterAutoClientConnectionTimeoutProcess()
{
	std::vector<AbstractComponentPtr> acp;
	registerComponentGroup.values(acp);
	//90s��ʱ
	const long long timeout{ 90000 }, nowTime{ Time().tickcount() };

	for (int i = 0; i != acp.size(); ++i)
	{
		if (timeout < nowTime - acp[i]->getComponentTimestamp())
		{
			removeComponentByID(acp[i]->getComponentID());
		}
	}
}

void MDCServer::sendRegisterWorkerServerMessage()
{
	std::string name, id;
	XMLParser().getValueByName("Config.xml", "Component.Dispatcher.ID", id);
	XMLParser().getValueByName("Config.xml", "Component.Dispatcher.Name", name);
	AbstractComponent component(
		base::component::ComponentType::COMPONENT_TYPE_XMQ);
	component.setComponentID(id);
	component.setComponentName(name);
	ownerXMQID = id;

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
		sendData("worker", "request", id, "", data);
	}
}

void MDCServer::processPolledDataFromServer(
	const std::string commID, 
	const std::string roleID, 
	const std::string flagID, 
	const std::string fromID, 
	const std::string toID, 
	const std::string data)
{
	//�����ֻ�����������Ϣ
	DataPacketPtr pkt{ DataParser().parseData(data) };

	if (pkt)
	{
		MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
		const base::packet::MessagePacketType type{ msgpkt->getMessagePacketType() };

		if (base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_COMPONENT == type)
		{
			processComponentMessage(commID, flagID, fromID, pkt);
		}
		else if (base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_ALARM == type)
		{
			//ֱ�Ӳ�ѯWEB���ID��ʶ������ת����Ϣ
			std::vector<AbstractComponentPtr> components;
			registerComponentGroup.values(components);

			for (int i = 0; i != components.size(); ++i)
			{
				if (base::component::ComponentType::COMPONENT_TYPE_WEB == components[i]->getComponentType())
				{
					forwardPolledDataFromServer(
						components[i]->getCommunicationID(),
						flagID,
						fromID,
						"",
						data);
				}
			}

			LOG(INFO) << "Push alarm message to WEB service.";
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

void MDCServer::forwardPolledDataFromServer(
	const std::string commID, 
	const std::string flagID, 
	const std::string fromID, 
	const std::string toID, 
	const std::string data)
{
	if (!flagID.compare("request") || !flagID.compare("notification"))
	{
		//�ֽ�toID�鿴��һ��·�ɵ�ַ
		std::vector<std::string> addressGroup;
		boost::split(addressGroup, toID, boost::is_any_of(":"));
		AbstractComponentPtr component{ registerComponentGroup.at(addressGroup[0]) };

		if (component)
		{
// 			const std::size_t pos{ toID.find_first_of(":") };
 			const std::string communicationID{ component->getCommunicationID() };
			int e{ 
				sendData(
					communicationID,
					"server",
					flagID, 
					fromID, 
					toID,//toID.substr(-1 < pos ? pos : 0, toID.length()), 
					data) };

			if (eSuccess == e)
			{
				LOG(INFO) << "Forward request message to component name = " << component->getComponentName()
					<< ", CommunicationID = " << communicationID
// 					<< ", FlagID = " << flagID
// 					<< ", fromID = " << fromID
// 					<< ", toID = " << toID
// 					<< ", length = " << msg.length() 
					<< ".";
			} 
			else
			{
				LOG(WARNING) << "Forward request message failed to component name = " << component->getComponentName()
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
			LOG(ERROR) << "Can not forward to component ID = " << toID << ".";
		}
	}
	else if (!flagID.compare("response"))
	{
		int e{ eSuccess };
		std::string componentName;
		AbstractComponentPtr component{ registerComponentGroup.at(toID) };

		//���toID�������ڷ����ת��
		//���toID���������ڿͻ���ת��
		if (component)
		{
			e = sendData(component->getCommunicationID(), "server", flagID, fromID, toID, data);
			componentName = component->getComponentName();
		} 
		else
		{
			//��Ҫ����fromID��toID
			//���Ͷ˷���ʱ����,ת���Ͳ�Ҫ������
			e = sendData("server", flagID, fromID, toID, data);
			componentName = "Upstream";
		}

		if (eSuccess == e)
		{
			LOG(INFO) << "Forward response message to component name = " << componentName
// 				<< ", FlagID = " << flagID
// 				<< ", fromID = " << fromID
// 				<< ", toID = " << toID
// 				<< ", length = " << msg.length() 
				<< ".";
		}
		else
		{
			LOG(WARNING) << "Forward response message failed to component name = " << componentName
// 				<< ", FlagID = " << flagID
// 				<< ", fromID = " << fromID
// 				<< ", toID = " << toID
// 				<< ", length = " << msg.length() 
				<< ".";
		}
	}
	else if (!flagID.compare("upload")/* && !fromID.compare(toID)*/)
	{
		std::vector<AbstractComponentPtr> acps;
		registerComponentGroup.values(acps);

		for (int i = 0; i != acps.size(); ++i)
		{
			if (base::component::ComponentType::COMPONENT_TYPE_WEB == acps[i]->getComponentType())
			{
				const std::string webcommid{ acps[i]->getCommunicationID() }, webcompid{ acps[i]->getComponentID() };
				sendData(
					webcommid,
					"server",
					flagID,
					fromID,
					webcompid,
					data);
				LOG(INFO) << "Forward upload alarm message to WEB Component ID = " << webcompid << 
					" , Communicateion ID = " << webcommid << ".";
			}
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
		//���IDΪ�ձ�ʾ�������ע��
		//���ID���ձ�ʾ�����������

		std::string componentID{ ac->getComponentID() };
		const std::string componentName{ ac->getComponentName() };
		const base::component::ComponentType componentType{ ac->getComponentType() };
// 		int e{ eSuccess };
// 
// 		if (componentID.empty())
// 		{
// 			componentID = boost::uuids::to_string(boost::uuids::random_generator()());
// 			//Ϊ��ע����������ID��ʶ
// 			e = addNewRegisterComponent(componentID, componentName, commID, componentType);
// 		}
// 		else
// 		{
// 			e = updateRegisterCompnent(componentID, componentName, commID, componentType);
// 		}

		//commID��fromIDһ��
		replyMessageWithResultAndExtendID(
			commID,
			AbstractWorker::getUUID(),
			fromID,
			static_cast<int>(base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_COMPONENT),
			static_cast<int>(base::protocol::ComponentCommand::COMPONENT_COMMAND_SIGNIN_REP),
			updateRegisterCompnent(componentID, componentName, commID, componentType),
			pkt->getPacketSequence() + 1);
	}
	else if (base::protocol::ComponentCommand::COMPONENT_COMMAND_SIGNIN_REP == command)
	{
// 		const char* componentID{
// 			reinterpret_cast<const char*>(pkt->getPacketData()) };
// 		//����ע�ỹ���������������ID��ʶ
// 		setMDCServerInfoWithName("Component.Disptcher.ID", componentID);

		parentXMQID = reinterpret_cast<const char*>(pkt->getPacketData());
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
				AbstractComponentPtr acp{ registerComponentGroup.at(fromID) };
				if (acp)
				{
					sendData(commID, "worker", "response", AbstractWorker::getUUID(), fromID, msgstr);
				} 
				else
				{
					sendData("worker", "response", AbstractWorker::getUUID(), fromID, msgstr);
				}
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
		//ƥ�����ID��ʶ�Ը������ʵ����ʱ�����ͨ��ID��ʶ
		//���ƥ�䵽���ID��ʶ���������

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
			e = sendData(commID, "server", "response", extendID, fromID, DataPacker().packData(datapkt));
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
