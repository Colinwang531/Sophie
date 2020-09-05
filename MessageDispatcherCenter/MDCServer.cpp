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
#include "Protocol/Component/ComponentPhrase.h"
//#include "Protocol/Device/DevicePhrase.h"
#include "Xml/XmlCodec.h"
using XMLParser = base::xml::XMLParser;
using XMLPacker = base::xml::XMLPacker;
#include "Packet/Message/MessagePacket.h"
using MessagePacket = base::packet::MessagePacket;
using MessagePacketPtr = boost::shared_ptr<MessagePacket>;
#include "Component/AbstractComponent.h"
using AbstractComponent = base::component::AbstractComponent;
// #include "Device/SurveillanceDevice.h"
// using SurveillanceDevice = base::device::SurveillanceDevice;
#include "MDCServer.h"

MDCServer::MDCServer(
	const ServerModuleType server/* = ServerModuleType::SERVER_MODULE_TYPE_NONE*/,
	const ClientModuleType upstream/* = ClientModuleType::CLIENT_MODULE_TYPE_NONE*/,
	const std::string address/* = "tcp:\\127.0.0.1:61001"*/,
	const std::string name/* = "MDCServer"*/)
	: AbstractUpstreamServer(server, upstream, address)
{
	setMDCServerInfoWithName("Component.XMQ.Name", name);
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
	LOG(INFO) << "Server polled message with CommID = " << commID
		<< ", FlagID = " << flagID
		<< ", fromID = " << fromID
		<< ", toID = " << toID
		<< ", length = " << msg.length() << ".";

	//toIDΪ�ձ�ʾ��Ϣ���ڵ�ǰ����˴���,
	//������toIDת����Ϣ
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
}

void MDCServer::afterAutoCheckConnectionTimeoutProcess()
{
	std::vector<AbstractComponentPtr> acp;
	registerComponentGroup.values(acp);
	//90s��ʱ
	const long long timeout{ 90000 }, nowTime{ Time().tickcount() };

	for (int i = 0; i != acp.size(); ++i)
	{
		if (timeout < nowTime - acp[i]->getComponentTimestamp())
		{
			registerComponentGroup.remove(acp[i]->getComponentID());
		}
	}
}

const std::string MDCServer::buildAutoRegisterToBrokerMessage()
{
	std::string msgstr;
	AbstractComponent component(
		base::component::ComponentType::COMPONENT_TYPE_XMQ);
	component.setComponentID(getMDCServerInfoByName("Component.XMQ.ID"));
	component.setComponentName(getMDCServerInfoByName("Component.XMQ.Name"));
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
	//�����ֻ����ע��/ע��/����/��ѯ��Ϣ
	DataPacketPtr pkt{ DataParser().parseData(msg) };

	if (pkt)
	{
		MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
		const base::packet::MessagePacketType type{ msgpkt->getMessagePacketType() };

// 		if (base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_ALARM == type)
// 		{
// 			//				forwardAlgorithmConfigureMessage(s, data, databytes);
// 		}
// 		else if (base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_ALGORITHM == type)
// 		{
// 			forwardAlgorithmConfigureMessage(commID, mm.release_component(), mm.sequence());
// 		}
// 		else if (base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_COMPONENT == type)
// 		{
// 			processComponentMessage(commID, pkt);
// 		}
// 		else if (base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_CREW == type)
// 		{
// 			forwardCrewConfigureMessage(msg, commID, mm.release_device(), mm.sequence());
// 		}
// 		else if (base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_DEVICE == type)
// 		{
// 			forwardDeviceMessage(commID, pkt, msg);
// 		}
// 		else if (base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_STATUS == type)
// 		{
// 			forwardStatusConfigureMessage(msg, commID, mm.release_status(), mm.sequence());
// 		}
// 		else if (base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_USER == type)
// 		{
// 		}

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
	if (!flagID.compare("request"))
	{
		//�ֽ�toID�鿴��һ��·�ɵ�ַ
		std::vector<std::string> addressGroup;
		boost::split(addressGroup, toID, boost::is_any_of(":"));

		if (registerComponentGroup.at(addressGroup[0]))
		{
			int e{ AbstractServer::sendMessageData(
				addressGroup[0], flagID, fromID, toID.substr(toID.find_first_of(":"), toID.length()), msg) };

			if (eSuccess == e)
			{
				LOG(INFO) << "Forward request message to next CommID = " << addressGroup[0]
					<< ", FlagID = " << flagID
					<< ", fromID = " << fromID
					<< ", toID = " << toID
					<< ", length = " << msg.length() << ".";
			} 
			else
			{
				LOG(WARNING) << "Forward request message failed to next CommID = " << addressGroup[0]
					<< ", FlagID = " << flagID
					<< ", fromID = " << fromID
					<< ", toID = " << toID
					<< ", length = " << msg.length() << ".";
			}
		} 
		else
		{
			LOG(ERROR) << "Can not forward to next address = " << addressGroup[0] << ".";
		}
	}
	else if (!flagID.compare("response"))
	{
		int e{ eSuccess };
		std::string addressID{ "Upstream" };

		//���toID�������ڷ����ת��
		//���toID���������ڿͻ���ת��
		if (registerComponentGroup.at(toID))
		{
			e = AbstractServer::sendMessageData(toID, flagID, fromID, toID, msg);
			addressID = toID;
		} 
		else
		{
			//��Ҫ����fromID��toID
			//���Ͷ˷���ʱ����,ת���Ͳ�Ҫ������
			e = AbstractClient::sendMessageData(flagID, fromID, toID, msg);
		}

		if (eSuccess == e)
		{
			LOG(INFO) << "Forward response message to next CommID = " << addressID
				<< ", FlagID = " << flagID
				<< ", fromID = " << fromID
				<< ", toID = " << toID
				<< ", length = " << msg.length() << ".";
		}
		else
		{
			LOG(WARNING) << "Forward response message failed to next CommID = " << addressID
				<< ", FlagID = " << flagID
				<< ", fromID = " << fromID
				<< ", toID = " << toID
				<< ", length = " << msg.length() << ".";
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

	if (ac)
	{
		if (base::protocol::ComponentCommand::COMPONENT_COMMAND_SIGNIN_REQ == command)
		{
			//���IDΪ�ձ�ʾ�������ע��
			//���ID���ձ�ʾ�����������

			std::string componentID{ ac->getComponentID() };
			const std::string componentName{ ac->getComponentName() };
			const base::component::ComponentType componentType{ ac->getComponentType() };
			int e{ eSuccess };

			if (componentID.empty())
			{
				componentID = boost::uuids::to_string(boost::uuids::random_generator()());
				//Ϊ��ע����������ID��ʶ
				e = addNewRegisterComponent(componentID, componentName, commID, componentType);
			}
			else
			{
				e = updateRegisterCompnent(componentID, componentName, commID, componentType);
			}

			//commID��fromIDһ��
			replyMessageWithResultAndExtendID(
				commID,
				componentID,
				fromID,
				static_cast<int>(base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_COMPONENT),
				static_cast<int>(base::protocol::ComponentCommand::COMPONENT_COMMAND_SIGNIN_REP),
				e,
				pkt->getPacketSequence() + 1);
		}
		else if (base::protocol::ComponentCommand::COMPONENT_COMMAND_SIGNOUT_REQ == command)
		{
			removeRegisterComponent(
				ac->getComponentID(),
				static_cast<base::component::ComponentType>(ac->getComponentType()));
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
					//commID��fromIDһ��
					AbstractServer::sendMessageData(commID, "response", "", fromID, msgstr);
				}
			}
		}
	}
}

// void MDCServer::forwardDeviceMessage(
// 	const std::string commID, 
// 	DataPacketPtr pkt, 
// 	const std::string msg)
// {
// 	//�豸��Ϣͨ����Ϣ������WEB������豸���֮�佻��
// 	//����Ϣ����ֻת����Ϣ��������
// 	//�����Ӧ���豸������������ɷ���˽���Ӧ��
// 
// 	int e{ eNotSupport };
// 	MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
// 	const base::protocol::DeviceCommand command{ 
// 		static_cast<base::protocol::DeviceCommand>(msgpkt->getMessagePacketCommand()) };
// 
// 	if (base::protocol::DeviceCommand::DEVICE_COMMAND_NEW_REQ == command ||
// 		base::protocol::DeviceCommand::DEVICE_COMMAND_DELETE_REQ == command ||
// 		base::protocol::DeviceCommand::DEVICE_COMMAND_MODIFY_REQ == command)
// 	{
// 		SurveillanceDevice* sd{ 
// 			reinterpret_cast<SurveillanceDevice*>(pkt->getPacketData()) };
// 		const base::device::SurveillanceDeviceFactory factory{ sd->getDeviceFactory() };
// 		
// 		if (base::device::SurveillanceDeviceFactory::SURVEILLANCE_DEVICE_FACTORY_HK == factory)
// 		{
// 			e = forwardMessageByComponentType(msg, base::component::ComponentType::COMPONENT_TYPE_HKD);
// 		}
// 		else if (base::device::SurveillanceDeviceFactory::SURVEILLANCE_DEVICE_FACTORY_DH == factory)
// 		{
// 			e = forwardMessageByComponentType(msg, base::component::ComponentType::COMPONENT_TYPE_DHD);
// 		}
// // 		else if (DeviceFactory::DEVICE_FACTORY_ET == dr->embedded.factory)
// // 		{
// // 		}
// 		
// 		if(eSuccess != e)
// 		{
// 			replyMessageWithResultAndExtendID(
// 				commID,
// 				"",
// 				static_cast<int>(base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_DEVICE),
// 				static_cast<int>(command),
// 				e,
// 				pkt->getPacketSequence() + 1);
// 		}
// 	}
// 	else if (base::protocol::DeviceCommand::DEVICE_COMMAND_NEW_REP == command ||
// 		base::protocol::DeviceCommand::DEVICE_COMMAND_DELETE_REP == command ||
// 		base::protocol::DeviceCommand::DEVICE_COMMAND_MODIFY_REP == command)
// 	{
// 		e = forwardMessageByComponentType(msg, base::component::ComponentType::COMPONENT_TYPE_WEB);
// 	}
// }
// 
// void MDCServer::forwardStatusConfigureMessage(
// 	const std::string fwdmsg, 
// 	const std::string commID, 
// 	void* status /* = nullptr */,
// 	const long long sequence /* = -1 */)
// {
// 	int e{ eNotSupport };
// 	msg::Status* ms{ reinterpret_cast<msg::Status*>(status) };
// 	const msg::Status_Command command{ ms->command() };
// 
// 	//���������޷���֪����Щ�豸�������,����ÿ�ζ�������ȫ�����͵��豸���ת��һ��
// 	//��û���豸�������ʱ�ɷ���˽���Ӧ��
// 
// 	if (msg::Status_Command::Status_Command_SET_REQ == command)
// 	{
// 		e = forwardMessageByComponentType(fwdmsg, base::component::ComponentType::COMPONENT_TYPE_AI);
// 
// 		if (eSuccess != e)
// 		{
// 			replyMessageWithResultAndExtendID(
// 				commID,
// 				"",
// 				static_cast<int>(base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_STATUS),
// 				static_cast<int>(command),
// 				e,
// 				sequence + 1);
// 		}
// 	}
// 	else if (msg::Status_Command::Status_Command_SET_REP == command)
// 	{
// 		e = forwardMessageByComponentType(fwdmsg, base::component::ComponentType::COMPONENT_TYPE_WEB);
// 	}
// }
// 
// void MDCServer::forwardAlgorithmConfigureMessage(
// 	const std::string fwdmsg, 
// 	const std::string commID, 
// 	void* algorithm /* = nullptr */,
// 	const long long sequence /* = -1 */)
// {
// 	int e{ eNotSupport };
// 	msg::Algorithm* ma{ reinterpret_cast<msg::Algorithm*>(algorithm) };
// 	const msg::Algorithm_Command command{ ma->command() };
// 
// 	//���������޷���֪�м����㷨�������,����ÿ�ζ�������ȫ�����͵��㷨���ת��һ��
// 	//��û���㷨�������ʱ�ɷ���˽���Ӧ��
// 
// 	if (msg::Algorithm_Command::Algorithm_Command_CONFIGURE_REQ == command)
// 	{
// 		e = forwardMessageByComponentType(fwdmsg, base::component::ComponentType::COMPONENT_TYPE_AI);
// 
// 		if (eSuccess != e)
// 		{
// 			replyMessageWithResultAndExtendID(
// 				commID,
// 				"",
// 				static_cast<int>(base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_ALGORITHM),
// 				static_cast<int>(command),
// 				e,
// 				sequence + 1);
// 		}
// 	}
// 	else if (msg::Algorithm_Command::Algorithm_Command_CONFIGURE_REP == command)
// 	{
// 		e = forwardMessageByComponentType(fwdmsg, base::component::ComponentType::COMPONENT_TYPE_WEB);
// 	}
// }
// 
// // void MDCServer::forwardAlarmInfoMessage(
// // 	void* s /* = nullptr */,
// // 	const void* data /* = nullptr */,
// // 	const unsigned int databytes /* = 0 */)
// // {
// // 	int e{
// // 		forwardConfigureRequestOrResponseMessage(
// // 			s, static_cast<int>(base::component::ComponentType::COMPONENT_TYPE_ALM), data, databytes) };
// // 
// // 	if (eSuccess == e)
// // 	{
// // 		LOG(INFO) << "Forward alarm message of algorithm successfully.";
// // 	}
// // 	else
// // 	{
// // 		LOG(WARNING) << "Forward alarm message of algorithm failed, result = " << e << ".";
// // 	}
// // }
// 
// void MDCServer::forwardCrewConfigureMessage(
// 	const std::string fwdmsg, 
// 	const std::string commID, 
// 	void* crew /* = nullptr */,
// 	const long long sequence /* = -1 */)
// {
// 	int e{ eNotSupport };
// 	msg::Crew* mc{ reinterpret_cast<msg::Crew*>(crew) };
// 	const msg::Crew_Command command{ mc->command() };
// 
// 	//���������޷���֪�м����㷨�������,����ÿ�ζ�������ȫ�����͵��㷨���ת��һ��
// 	//��û���㷨�������ʱ�ɷ���˽���Ӧ��
// 
// 	if (msg::Crew_Command::Crew_Command_NEW_REQ == command ||
// 		msg::Crew_Command::Crew_Command_DELETE_REQ == command ||
// 		msg::Crew_Command::Crew_Command_MODIFY_REQ == command)
// 	{
// 		e = forwardMessageByComponentType(fwdmsg, base::component::ComponentType::COMPONENT_TYPE_AI);
// 
// 		if (eSuccess != e)
// 		{
// 			replyMessageWithResultAndExtendID(
// 				commID,
// 				"",
// 				static_cast<int>(base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_CREW),
// 				static_cast<int>(command),
// 				e,
// 				sequence + 1);
// 		}
// 	}
// 	else if (msg::Crew_Command::Crew_Command_NEW_REP == command ||
// 		msg::Crew_Command::Crew_Command_DELETE_REP == command ||
// 		msg::Crew_Command::Crew_Command_MODIFY_REP == command)
// 	{
// 		e = forwardMessageByComponentType(fwdmsg, base::component::ComponentType::COMPONENT_TYPE_WEB);
// 	}
// }

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
		registerComponentGroup.insert(communicationID, acp);
		e = eSuccess;

		LOG(INFO) << "Add new register component type = " << static_cast<int>(componentType) <<
			", ID = " << componentID <<
			", CommID = " << communicationID <<
			", Name = " << serviceName <<
			", Timestamp = " << acp->getComponentTimestamp() << ".";
	} 
	else
	{
		LOG(WARNING) << "Failed to add new register component type = " << static_cast<int>(componentType) <<
			", ID = " << componentID <<
			", CommID = " << communicationID <<
			", Name = " << serviceName << ".";
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
		//������ҵ�ƥ���ID��ʶ��ִ�и��²���
		//���û��ƥ���ID��ʶ��ִ����������

		AbstractComponentPtr acp{ registerComponentGroup.at(communicationID) };

		if (acp)
		{
			acp->setComponentTimestamp(Time().tickcount());
			LOG(INFO) << "Update information of component, type = " << static_cast<int>(componentType) <<
				", CommID = " << communicationID << ".";
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

		if (base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_COMPONENT == static_cast<base::packet::MessagePacketType>(pkttype))
		{
			e = AbstractServer::sendMessageData(commID, "response", "", fromID, DataPacker().packData(datapkt));
		}
// 		else if (base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_DEVICE == static_cast<base::packet::MessagePacketType>(pkttype))
// 		{
// 			e = sendMessageData(commID, DevicePacker().packData(datapkt));
// 		}
	}

	return e;
}

int MDCServer::removeRegisterComponent(
	const std::string componentID,
	const base::component::ComponentType componentType/* = base::component::ComponentType::COMPONENT_TYPE_NONE*/)
{
	int e{ !componentID.empty() && base::component::ComponentType::COMPONENT_TYPE_NONE != componentType ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		AbstractComponentPtr acp{ registerComponentGroup.at(componentID) };

		if (acp && componentType == acp->getComponentType())
		{
			registerComponentGroup.remove(componentID);

			LOG(WARNING) << "Remove register component type = " << static_cast<int>(componentType) <<
				", ID = " << componentID <<
				", Name = " << acp->getComponentName() <<
				", Timestamp = " << acp->getComponentTimestamp() <<
				"[ Expire = " << Time().tickcount() - acp->getComponentTimestamp() << " ].";
		}
		else
		{
			e = eBadOperate;
			LOG(WARNING) << "Can not find register component type = " << static_cast<int>(componentType) <<
				", ID = " << componentID << ".";
		}
	}

	return e;
}

// int MDCServer::forwardMessageByComponentType(
// 	const std::string fwdmsg,
// 	const base::component::ComponentType componentType /* = base::component::ComponentType::COMPONENT_TYPE_NONE */)
// {
// 	//�Ȳ��Ҷ�Ӧ�������ת����Ϣ
// 	//ÿ�β��������������������,�����������������������Ϣ
// 
// 	int e{ !fwdmsg.empty() && base::component::ComponentType::COMPONENT_TYPE_NONE != componentType ? eSuccess : eInvalidParameter };
// 
// 	if (eSuccess == e)
// 	{
// 		e = eNotSupport;
// 		std::vector<AbstractComponentPtr> components;
// 		registerComponentGroup.values(components);
// 
// 		for (int i = 0; i != components.size(); ++i)
// 		{
// 			if (componentType == components[i]->getComponentType())
// 			{
// 				e = AbstractServer::sendMessageData(components[i]->getCommunicationID(), fwdmsg);
// 
// 				if (eSuccess == e)
// 				{
// 					LOG(INFO) << "Forward message to component successfully, type = " << static_cast<int>(componentType) << ".";
// 				}
// 				else
// 				{
// 					LOG(WARNING) << "Forward message to component failed, result = " << e << ".";
// 				}
// 			}
// 		}
// 	}
// 
// 	return e;
// }
