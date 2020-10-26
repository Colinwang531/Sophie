#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
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
#include "ASIO/TCPConnector.h"
using TCPConnector = base::network::TCPConnector;
using TCPConnectorPtr = boost::shared_ptr<TCPConnector>;
#include "Protocol/DataPhrase.h"
using DataParser = base::protocol::DataParser;
using DataPacker = base::protocol::DataPacker;
#include "Protocol/ComponentPhrase.h"
#include "Protocol/AlgorithmPhrase.h"
#include "Protocol/StatusPhrase.h"
#include "Protocol/AlarmPhrase.h"
#include "Xml/XmlCodec.h"
using XMLParser = base::xml::XMLParser;
using XMLPacker = base::xml::XMLPacker;
#include "Component/AbstractComponent.h"
using AbstractComponent = base::component::AbstractComponent;
#include "AI/AbstractAlgorithm.h"
using AbstractAlgorithm = base::ai::AbstractAlgorithm;
#include "Packet/Message/MessagePacket.h"
using DataPacket = base::packet::DataPacket;
using DataPacketPtr = boost::shared_ptr<DataPacket>;
using MessagePacket = base::packet::MessagePacket;
using MessagePacketPtr = boost::shared_ptr<MessagePacket>;
#include "PhoneDetectComponentClient.h"

PhoneDetectComponentClient::PhoneDetectComponentClient(
	const std::string address, 
	const unsigned short port /* = 60531 */)
	: AbstractWorker(), sailStatus{ -1 }, mediaIP{ address }, mediaPort{ port }
{}
PhoneDetectComponentClient::~PhoneDetectComponentClient() {}

int PhoneDetectComponentClient::createNewClient(const std::string address)
{
	int e{ !address.empty() ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		MajordomoWorkerPtr mdwp{
			boost::make_shared<MajordomoWorker>(
				boost::bind(&PhoneDetectComponentClient::afterPolledDataFromWorkerCallback, this, _1, _2, _3, _4, _5)) };
		if (mdwp && eSuccess == mdwp->startWorker(address))
		{
			worker.swap(mdwp);
			//在客户端注册或心跳之前创建UUID标识
			AbstractWorker::generateUUIDWithName("Phone");
			e = AbstractWorker::createNewClient("");
			asioService.startService();
		}
		else
		{
			e = eBadNewObject;
		}
	}

	return e;
}

int PhoneDetectComponentClient::destroyClient()
{
	asioService.stopService();
	return worker ? worker->stopWorker() : eBadOperate;
}

void PhoneDetectComponentClient::afterPolledDataFromWorkerCallback(
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
			processComponentMessage(fromID, pkt);
		}
		else if (base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_ALGORITHM == type)
		{
			processAlgorithmMessage(fromID, pkt);
		}
		else if (base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_STATUS == type)
		{
			processStatusMessage(fromID, pkt);
		}
		else
		{
			LOG(INFO) << "Parse phone client polled message with unknown type = " << static_cast<int>(type) << ".";
		}
	}
	else
	{
		LOG(ERROR) << "Parse phone client polled message failed.";
	}
}

void PhoneDetectComponentClient::sendRegisterWorkerServerMessage()
{
	std::string name, id;
	XMLParser().getValueByName("Config.xml", "Component.Phone.ID", id);
	XMLParser().getValueByName("Config.xml", "Component.Phone.Name", name);
	AbstractComponent component(
		base::component::ComponentType::COMPONENT_TYPE_AI);
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

void PhoneDetectComponentClient::sendQuerySystemServiceMessage()
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
		sendData("worker", "request", AbstractWorker::getUUID(), parentXMQID, data);
	}
}

int PhoneDetectComponentClient::sendData(
	const std::string roleID,
	const std::string flagID,
	const std::string fromID,
	const std::string toID,
	const std::string data)
{
	return worker ? worker->sendData(roleID, flagID, fromID, toID, data) : eBadOperate;
}

int PhoneDetectComponentClient::createNewMediaStreamSession(
	const std::string url,
	boost::asio::ip::tcp::socket* s)
{
	int e{ !url.empty() && s ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		AbstractAlgorithm algo{ algorithmParamGroup.at() };
		algorithmParamGroup.removeFront();
		TCPSessionPtr session{
			boost::make_shared<PhoneMediaStreamSession>(*this, AbstractWorker::getUUID(), url, algo, s) };

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

const std::string PhoneDetectComponentClient::getAlgorithmClientInfoByName(const std::string name) const
{
	char exeFullPath[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, exeFullPath, MAX_PATH);
	std::string tempExeFullPath{ exeFullPath };
	std::string exeDirPath{ tempExeFullPath.substr(0, tempExeFullPath.rfind("\\", tempExeFullPath.length())) };
	std::string value;
	XMLParser().getValueByName(exeDirPath + "\\Config.xml", name, value);
	return value;
}

void PhoneDetectComponentClient::setAlgorithmClientInfoWithName(
	const std::string name, const std::string value)
{
	char exeFullPath[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, exeFullPath, MAX_PATH);
	std::string tempExeFullPath{ exeFullPath };
	std::string exeDirPath{ tempExeFullPath.substr(0, tempExeFullPath.rfind("\\", tempExeFullPath.length())) };
	XMLPacker().setValueWithName(exeDirPath + "\\Config.xml", name, value);
}

void PhoneDetectComponentClient::processComponentMessage(
	const std::string fromID, 
	DataPacketPtr pkt)
{
	MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
	const base::protocol::ComponentCommand command{
		static_cast<base::protocol::ComponentCommand>(msgpkt->getMessagePacketCommand()) };

	if (base::protocol::ComponentCommand::COMPONENT_COMMAND_SIGNIN_REP == command)
	{
// 		const char* componentID{ 
// 			reinterpret_cast<const char*>(pkt->getPacketData()) };
// 		//无论注册还是心跳都保存组件ID标识
// 		setAlgorithmClientInfoWithName("Component.Phone.ID", componentID);

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
				if (base::component::ComponentType::COMPONENT_TYPE_ALM == type)
				{
					//保存报警子服务组件ID标识
					//通过该ID标识和报警子服务组件进行报警数据发送
					const std::string id{ ac->getCommunicationID() };
					std::vector<TCPSessionPtr> sessions;
					streamSessionGroup.values(sessions);

					if (!id.empty())
					{
						for (int i = 0; i != sessions.size(); ++i)
						{
							PhoneStreamSessionPtr session{ 
								boost::dynamic_pointer_cast<PhoneMediaStreamSession>(sessions[i]) };
							session->setAlarmCommunicationID(id);
						}
					}
				}
			} 
			else
			{
				break;
			}
		}
	}
}

void PhoneDetectComponentClient::processAlgorithmMessage(const std::string fromID, DataPacketPtr pkt)
{
	MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
	const base::protocol::AlgorithmCommand command{
		static_cast<base::protocol::AlgorithmCommand>(msgpkt->getMessagePacketCommand()) };
	AbstractAlgorithm* aa{ reinterpret_cast<AbstractAlgorithm*>(pkt->getPacketData()) };
	int e{ fromID.empty() || !pkt ? eInvalidParameter : eSuccess };

	if (eSuccess == e && aa)
	{
		if (base::protocol::AlgorithmCommand::ALGORITHM_COMMAND_CONFIG_REQ == command)
		{
			AbstractAlgorithm* aa{ reinterpret_cast<AbstractAlgorithm*>(pkt->getPacketData()) };
			const std::string streamID{ aa->getStreamID() };

			if (!streamID.empty())
			{
				algorithmParamGroup.pushBack(*aa);
				std::vector<std::string> streamIDGroup;
				boost::split(streamIDGroup, streamID, boost::is_any_of(":"));
				connectMediaServer(streamIDGroup[0], streamIDGroup[1], atoi(streamIDGroup[2].c_str()));

				LOG(INFO) << "Deploy algorithm on stream ID = " << streamID << 
					" GPU = " << aa->getGpuID() << 
					" first threashold = " << aa->getFirstDetectThreshold() << 
					" second threashold = " << aa->getSecondDetectThreshold() << 
					" track threashold = " << aa->getTrackThreshold() << ".";
			}
			else
			{
				e = eNotSupport;
			}
		}
	}
	else
	{
		e = eBadOperate;
	}

	e = replyMessageWithResult(
		fromID,
		AbstractWorker::getUUID(),
		static_cast<int>(base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_ALGORITHM),
		static_cast<int>(command) + 1,
		e,
		pkt->getPacketSequence() + 1);
}

void PhoneDetectComponentClient::processStatusMessage(const std::string fromID, DataPacketPtr pkt)
{
	MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
	const base::protocol::StatusCommand command{
		static_cast<base::protocol::StatusCommand>(msgpkt->getMessagePacketCommand()) };

	if (base::protocol::StatusCommand::STATUS_COMMAND_SET_REQ == command)
	{
		sailStatus = msgpkt->getMessageStatus();

		LOG(INFO) << "Set algorithm enable status = " << sailStatus << ".";
	}

	replyMessageWithResult(
		fromID,
		AbstractWorker::getUUID(),
		static_cast<int>(base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_STATUS),
		static_cast<int>(command) + 1,
		eSuccess,
		pkt->getPacketSequence() + 1);
}

int PhoneDetectComponentClient::replyMessageWithResult(
	const std::string fromID, 
	const std::string toID,
	const int type /* = 0 */, 
	const int command /* = 0 */, 
	const int result /* = 0 */, 
	const long long sequence /* = 0 */)
{
	int e{ eBadNewObject };

	boost::shared_ptr<DataPacket> pkt{
		boost::make_shared<MessagePacket>(static_cast<base::packet::MessagePacketType>(type)) };

	if (pkt)
	{
		MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
		msgpkt->setMessagePacketCommand(command);
		msgpkt->setMessageStatus(result);
		msgpkt->setPacketSequence(sequence);
		const std::string rep{ DataPacker().packData(pkt) };

		//客户端应答时必须交换fromID和toID
		e = sendData("worker", "response", toID, fromID, rep);
	}

	return e;
}

int PhoneDetectComponentClient::connectMediaServer(
	const std::string did,
	const std::string cid,
	const int idx /* = -1 */)
{
	int e{ mediaIP.empty() || cid.empty() || did.empty() || 0 > idx ? eInvalidParameter : eSuccess };

	if (eSuccess == e)
	{
		TCPConnectorPtr connector{
			boost::make_shared<TCPConnector>(
				asioService,
				boost::bind(&PhoneDetectComponentClient::afterRemoteConnectedNotificationCallback, this, _1, _2)) };
		if (connector)
		{
			urlGroup.pushBack((boost::format("%s:%d") % did % idx).str());
			e = connector->setConnect(mediaIP.c_str(), mediaPort);
		}
		else
		{
			e = eBadNewObject;
		}
	}

	return e;
}

int PhoneDetectComponentClient::disconnectMediaServer(const std::string did)
{
	int e{ did.empty() ? eInvalidParameter : eSuccess };

	if (eSuccess == e)
	{
	}

	return e;
}

void PhoneDetectComponentClient::afterRemoteConnectedNotificationCallback(
	boost::asio::ip::tcp::socket* s, const boost::system::error_code e)
{
	if (s && !e)
	{
		const std::string url{ urlGroup.at() };
		if (!url.empty())
		{
			urlGroup.removeFront();
			createNewMediaStreamSession(url, s);
		}
	}
}
