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
#include "HelmetDetectComponentClient.h"

HelmetDetectComponentClient::HelmetDetectComponentClient(
	const std::string address, 
	const unsigned short port /* = 60531 */)
	: AbstractMediaStreamClient(address, port), sailStatus{ -1 }
{}
HelmetDetectComponentClient::~HelmetDetectComponentClient() {}

void HelmetDetectComponentClient::afterClientPolledMessageProcess(
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
			LOG(INFO) << "Parse helmet client polled message with unknown type = " << static_cast<int>(type) << ".";
		}
	}
	else
	{
		LOG(ERROR) << "Parse helmet client polled message failed.";
	}
}

const std::string HelmetDetectComponentClient::buildAutoRegisterToBrokerMessage()
{
	std::string msgstr;
	AbstractComponent component(
		base::component::ComponentType::COMPONENT_TYPE_AI);
	component.setComponentID(getAlgorithmClientInfoByName("Component.Helmet.ID"));
	component.setComponentName(getAlgorithmClientInfoByName("Component.Helmet.Name"));
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

const std::string HelmetDetectComponentClient::buildAutoQueryRegisterSubroutineMessage()
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

int HelmetDetectComponentClient::createNewMediaStreamSession(
	const std::string url,
	boost::asio::ip::tcp::socket* s)
{
	int e{ !url.empty() && s ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		AbstractAlgorithm algo{ algorithmParamGroup.at() };
		algorithmParamGroup.removeFront();
		TCPSessionPtr session{
			boost::make_shared<HelmetMediaStreamSession>(*this, url, algo, s) };

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

const std::string HelmetDetectComponentClient::getAlgorithmClientInfoByName(const std::string name) const
{
	char exeFullPath[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, exeFullPath, MAX_PATH);
	std::string tempExeFullPath{ exeFullPath };
	std::string exeDirPath{ tempExeFullPath.substr(0, tempExeFullPath.rfind("\\", tempExeFullPath.length())) };
	std::string value;
	XMLParser().getValueByName(exeDirPath + "\\Config.xml", name, value);
	return value;
}

void HelmetDetectComponentClient::setAlgorithmClientInfoWithName(
	const std::string name, const std::string value)
{
	char exeFullPath[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, exeFullPath, MAX_PATH);
	std::string tempExeFullPath{ exeFullPath };
	std::string exeDirPath{ tempExeFullPath.substr(0, tempExeFullPath.rfind("\\", tempExeFullPath.length())) };
	XMLPacker().setValueWithName(exeDirPath + "\\Config.xml", name, value);
}

void HelmetDetectComponentClient::processComponentMessage(
	const std::string fromID, 
	DataPacketPtr pkt)
{
	MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
	const base::protocol::ComponentCommand command{
		static_cast<base::protocol::ComponentCommand>(msgpkt->getMessagePacketCommand()) };

	if (base::protocol::ComponentCommand::COMPONENT_COMMAND_SIGNIN_REP == command)
	{
		const char* componentID{ 
			reinterpret_cast<const char*>(pkt->getPacketData()) };
		//无论注册还是心跳都保存组件ID标识
		setAlgorithmClientInfoWithName("Component.Helmet.ID", componentID);
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
					const std::string id{ ac->getComponentID() };
					std::vector<TCPSessionPtr> sessions;
					streamSessionGroup.values(sessions);

					if (!id.empty())
					{
						for (int i = 0; i != sessions.size(); ++i)
						{
							HelmetStreamSessionPtr session{ 
								boost::dynamic_pointer_cast<HelmetMediaStreamSession>(sessions[i]) };
							session->setAlarmComponentID(id);
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

void HelmetDetectComponentClient::processAlgorithmMessage(const std::string fromID, DataPacketPtr pkt)
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
				AbstractMediaStreamClient::connectMediaServerWithID(streamIDGroup[0], streamIDGroup[1], atoi(streamIDGroup[2].c_str()));

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
		static_cast<int>(base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_ALGORITHM),
		static_cast<int>(command) + 1,
		e,
		pkt->getPacketSequence() + 1);
}

void HelmetDetectComponentClient::processStatusMessage(const std::string fromID, DataPacketPtr pkt)
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
		static_cast<int>(base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_STATUS),
		static_cast<int>(command) + 1,
		eSuccess,
		pkt->getPacketSequence() + 1);
}

int HelmetDetectComponentClient::replyMessageWithResult(
	const std::string fromID, 
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
		e = AbstractClient::sendMessageData("response", "", fromID, rep);
	}

	return e;
}
