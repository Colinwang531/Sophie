#include <windows.h>
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

HelmetDetectComponentClient::HelmetDetectComponentClient()
	: AbstractClient(base::network::ClientModuleType::CLIENT_MODULE_TYPE_MAJORDOMO_WORKER), sailStatus{ -1 }
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
			processComponentMessage(pkt);
		}
		else if (base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_DEVICE == type)
		{
			processAlgorithmMessage(fromID, pkt);
		}
		else if (base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_STATUS == type)
		{
			processStatusMessage(fromID, pkt);
		}
		else
		{
			LOG(INFO) << "Parse HKD client polled message with unknown type = " << static_cast<int>(type) << ".";
		}
	}
	else
	{
		LOG(ERROR) << "Parse HKD client polled message failed.";
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

void HelmetDetectComponentClient::processComponentMessage(DataPacketPtr pkt)
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
		setAlgorithmClientInfoWithName("Component.Helmet.ID", componentID);
	}
}

void HelmetDetectComponentClient::processAlgorithmMessage(const std::string fromID, DataPacketPtr pkt)
{
	MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
	const base::protocol::AlgorithmCommand command{
		static_cast<base::protocol::AlgorithmCommand>(msgpkt->getMessagePacketCommand()) };
	AbstractAlgorithm* aa{ reinterpret_cast<AbstractAlgorithm*>(pkt->getPacketData()) };
	int e{ eBadOperate };

	if (aa)
	{
		if (base::protocol::AlgorithmCommand::ALGORITHM_COMMAND_SET_REQ == command)
		{
		}
	}

	e = replyMessageWithResult(
		fromID,
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
	}

	replyMessageWithResult(
		fromID,
		static_cast<int>(command) + 1,
		eSuccess,
		pkt->getPacketSequence() + 1);
}


// int HKDComponentClient::addNewDevice(SurveillanceDevice* sd /* = nullptr */)
// {
// 	int e{ eBadNewObject };
// 	const std::string did{ sd->getDeviceID() };
// 	const base::device::SurveillanceDeviceType dt{ sd->getDeviceType() };
// 	const std::string address{ sd->getDeviceIPv4Address() }, name{ sd->getLoginUserName() }, password{ sd->getLoginUserPassword() };
// 	const unsigned short port{ sd->getDevicePortNumber() };
// 
// 	AbstractDevicePtr adp{
// 		boost::make_shared<HikvisionDevice>(did, dt) };
// 	if (adp)
// 	{
// 		boost::shared_ptr<SurveillanceDevice> sdp{ 
// 			boost::dynamic_pointer_cast<SurveillanceDevice>(adp) };
// 		sdp->setDeviceIPv4Address(address);
// 		sdp->setDevicePortNumber(port);
// 		sdp->setLoginUserName(name);
// 		sdp->setLoginUserPassword(password);
// 		e = adp->startDevice();
// 
// 		if (eSuccess == e)
// 		{
// 			deviceGroup.insert(did, adp);
// 			LOG(INFO) << "Start new device successfully with address = " << address <<
// 				", port = " << port <<
// 				", name = " << name <<
// 				", password = " << password <<
// 				", ID = " << did << ".";
// 		}
// 		else
// 		{
// 			LOG(WARNING) << "Start new device failed with address = " << address <<
// 				", port = " << port <<
// 				", name = " << name <<
// 				", password = " << password <<
// 				", ID = " << did << ".";
// 		}
// 	}
// 	else
// 	{
// 		LOG(ERROR) << "Can not alloc memory for creating new device.";
// 	}
// 
// 	return e;
// }
// 
// int HKDComponentClient::deleteDeviceByID(const std::string did)
// {
// 	int e{ eOutOfRange };
// 	AbstractDevicePtr device{ deviceGroup.at(did) };
// 
// 	if (device)
// 	{
// 		e = device->stopDevice();
// 		if (eSuccess == e)
// 		{
// 			deviceGroup.remove(did);
// 		}
// 		
// 		LOG(INFO) << "Remove device ID = " << did << ".";
// 	}
// 	else
// 	{
// 		LOG(WARNING) << "Can not find device ID = " << did << ".";
// 	}
// 
// 	return e;
// }

int HelmetDetectComponentClient::replyMessageWithResult(
	const std::string fromID, 
	const int command /* = 0 */, 
	const int result /* = 0 */, 
	const long long sequence /* = 0 */)
{
	int e{ eBadNewObject };

	boost::shared_ptr<DataPacket> pkt{
		boost::make_shared<MessagePacket>(base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_DEVICE) };

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
