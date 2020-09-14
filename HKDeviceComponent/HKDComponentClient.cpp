#include "boost/pointer_cast.hpp"
#include "boost/make_shared.hpp"
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
#include "Protocol/DevicePhrase.h"
#include "Xml/XmlCodec.h"
using XMLParser = base::xml::XMLParser;
using XMLPacker = base::xml::XMLPacker;
#include "Component/AbstractComponent.h"
using AbstractComponent = base::component::AbstractComponent;
#include "Packet/Message/MessagePacket.h"
using DataPacket = base::packet::DataPacket;
using DataPacketPtr = boost::shared_ptr<DataPacket>;
using MessagePacket = base::packet::MessagePacket;
using MessagePacketPtr = boost::shared_ptr<MessagePacket>;
#include "HKDComponentClient.h"

HKDComponentClient::HKDComponentClient()
	: AbstractClient(base::network::ClientModuleType::CLIENT_MODULE_TYPE_MAJORDOMO_WORKER)
{}
HKDComponentClient::~HKDComponentClient() {}

void HKDComponentClient::afterClientPolledMessageProcess(
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
			processDeviceMessage(fromID, pkt);
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

const std::string HKDComponentClient::buildAutoRegisterToBrokerMessage()
{
	std::string msgstr;
	AbstractComponent component(
		base::component::ComponentType::COMPONENT_TYPE_HKD);
	component.setComponentID(getHKDClientInfoByName("Component.HKD.ID"));
	component.setComponentName(getHKDClientInfoByName("Component.HKD.Name"));
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

const std::string HKDComponentClient::getHKDClientInfoByName(const std::string name) const
{
	std::string value;
	XMLParser().getValueByName("Config.xml", name, value);
	return value;
}

void HKDComponentClient::setHKDClientInfoWithName(
	const std::string name, const std::string value)
{
	XMLPacker().setValueWithName("Config.xml", name, value);
}

void HKDComponentClient::processComponentMessage(DataPacketPtr pkt)
{
	MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
	const base::protocol::ComponentCommand command{
		static_cast<base::protocol::ComponentCommand>(msgpkt->getMessagePacketCommand()) };
//	AbstractComponent* ac{ reinterpret_cast<AbstractComponent*>(pkt->getPacketData()) };

	if (base::protocol::ComponentCommand::COMPONENT_COMMAND_SIGNIN_REP == command)
	{
		const char* componentID{
			reinterpret_cast<const char*>(pkt->getPacketData()) };
		//����ע�ỹ���������������ID��ʶ
		setHKDClientInfoWithName("Component.HKD.ID", componentID);
	}
}

void HKDComponentClient::processDeviceMessage(const std::string fromID, DataPacketPtr pkt)
{
	MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
	const base::protocol::DeviceCommand command{
		static_cast<base::protocol::DeviceCommand>(msgpkt->getMessagePacketCommand()) };
	SurveillanceDevice* sd{ reinterpret_cast<SurveillanceDevice*>(pkt->getPacketData()) };
	int e{ eBadOperate };
	std::vector<AbstractCamera> cameras;

	if (sd)
	{
		if (base::protocol::DeviceCommand::DEVICE_COMMAND_NEW_REQ == command)
		{
			e = addNewDevice(sd, cameras);
		}
		else if (base::protocol::DeviceCommand::DEVICE_COMMAND_DELETE_REQ == command)
		{
			e = deleteDeviceByID(sd->getDeviceID());
		}
		else if (base::protocol::DeviceCommand::DEVICE_COMMAND_MODIFY_REQ == command)
		{
			if (eSuccess == deleteDeviceByID(sd->getDeviceID()))
			{
				e = addNewDevice(sd, cameras);
			}
		}
	}

	e = replyMessageWithResult(
		fromID,
		static_cast<int>(command) + 1,
		e,
		pkt->getPacketSequence() + 1,
		cameras,
		sd->getDeviceID());
}

int HKDComponentClient::addNewDevice(
	SurveillanceDevice* sd, 
	std::vector<AbstractCamera>& cameras)
{
	int e{ eBadNewObject };
	const std::string did{ sd->getDeviceID() };
	const base::device::SurveillanceDeviceType dt{ sd->getDeviceType() };
	const std::string address{ sd->getDeviceIPv4Address() }, name{ sd->getLoginUserName() }, password{ sd->getLoginUserPassword() };
	const unsigned short port{ sd->getDevicePortNumber() };

	AbstractDevicePtr adp{
		boost::make_shared<HikvisionDevice>(did, dt) };
	if (adp)
	{
		SurveillanceDevicePtr sdp{ 
			boost::dynamic_pointer_cast<SurveillanceDevice>(adp) };
		sdp->setDeviceIPv4Address(address);
		sdp->setDevicePortNumber(port);
		sdp->setLoginUserName(name);
		sdp->setLoginUserPassword(password);
		e = adp->startDevice();

		if (eSuccess == e)
		{
			sdp->getDeviceCamera(cameras);
			deviceGroup.insert(did, adp);
			LOG(INFO) << "Start new device successfully with address = " << address <<
				", port = " << port <<
				", name = " << name <<
				", password = " << password <<
				", ID = " << did << ".";
		}
		else
		{
			LOG(WARNING) << "Start new device failed with address = " << address <<
				", port = " << port <<
				", name = " << name <<
				", password = " << password <<
				", ID = " << did << ".";
		}
	}
	else
	{
		LOG(ERROR) << "Can not alloc memory for creating new device.";
	}

	return e;
}

int HKDComponentClient::deleteDeviceByID(const std::string did)
{
	int e{ eOutOfRange };
	AbstractDevicePtr device{ deviceGroup.at(did) };

	if (device)
	{
		e = device->stopDevice();
		if (eSuccess == e)
		{
			deviceGroup.remove(did);
		}
		
		LOG(INFO) << "Remove device ID = " << did << ".";
	}
	else
	{
		LOG(WARNING) << "Can not find device ID = " << did << ".";
	}

	return e;
}

int HKDComponentClient::replyMessageWithResult(
	const std::string fromID, 
	const int command, 
	const int result, 
	const long long sequence, 
	const std::vector<AbstractCamera>& cameras,
	const std::string did)
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
		if (0 < cameras.size())
		{
			pkt->setPacketData((void*)&cameras);
		}
		pkt->setPacketData((void*)did.c_str());
		const std::string rep{ DataPacker().packData(pkt) };

		//�ͻ���Ӧ��ʱ���뽻��fromID��toID
		e = AbstractClient::sendMessageData("response", "", fromID, rep);
	}

	return e;
}
