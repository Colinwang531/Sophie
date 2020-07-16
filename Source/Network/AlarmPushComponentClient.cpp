#ifdef _WINDOWS
#include "glog/log_severity.h"
#include "glog/logging.h"
#else
#include "glog/log_severity.h"
#include "glog/logging.h"
#endif//_WINDOWS
#include "Error.h"
#include "Protocol/MessageDefs.h"
#include "Xml/XmlCodec.h"
using XMLPacker = base::xml::XMLPacker;
#include "HKDCClient.h"

HKDCClient::HKDCClient(
	const std::string cid,
	const std::string cname,
	CreateNewDeviceCallback ccb,
	DestroyDeviceCallback ddc)
	: NetworkAsynchronousClient(), clientType{ static_cast<int>(ComponentType::COMPONENT_TYPE_HKD) },
	clientID{ cid }, clientName{ cname }, createNewDeviceCallback{ ccb }, destroyDeviceCallback{ ddc }
{}
HKDCClient::~HKDCClient() {}

int HKDCClient::getClientType() const
{
	return clientType;
}

const char* HKDCClient::getClientID() const
{
	return clientID.c_str();
}

const char* HKDCClient::getClientName() const
{
	return clientName.c_str();
}

void HKDCClient::unpackMessageFailure()
{
	LOG(ERROR) << "Unpack message failed.";
}

void HKDCClient::parseUnknownMessage()
{
	LOG(WARNING) << "Parsed a unknown message and nothing to do.";
}

void HKDCClient::parseComponentMessage(void* pkt /* = nullptr */)
{
	MessagePacket* mp{ reinterpret_cast<MessagePacket*>(pkt) };
	ComponentPacket* cp{ reinterpret_cast<ComponentPacket*>(mp->packet) };
	ComponentResponse* cr{ reinterpret_cast<ComponentResponse*>(cp->data) };
	clientID = cr->cid;
	//保存ID标识以供重启使用
	XMLPacker().setValueWithName("Config.xml", "component.HKD", clientID);

	LOG(INFO) << "Parsed component message type = " << static_cast<int>(mp->type) << 
		", sequence = " << mp->sequence << 
		", timestamp = " << mp->timestamp <<
		", ID = " << clientID <<
		", response result = " << cr->result << ".";
}

void HKDCClient::parseDeviceMessage(void* pkt /* = nullptr */)
{
	MessagePacket* mp{ reinterpret_cast<MessagePacket*>(pkt) };
	DevicePacket* dp{ reinterpret_cast<DevicePacket*>(mp->packet) };

	if (MessageType::MESSAGE_TYPE_DEVICE == mp->type)
	{
		DeviceRequest* dr{ reinterpret_cast<DeviceRequest*>(dp->data) };

		if (DeviceCommand::DEVICE_COMMAND_NEW_REQ == dp->command)
		{
			if (createNewDeviceCallback)
			{
				int e{ 
					createNewDeviceCallback(
						dr->did, dr->embedded.ip, dr->embedded.port, dr->embedded.name, dr->embedded.password) };
			}
		} 
		else if (DeviceCommand::DEVICE_COMMAND_DELETE_REQ == dp->command)
		{
			if (destroyDeviceCallback)
			{
				int e{ destroyDeviceCallback(dr->did) };
			}
		}
		else
		{
			LOG(WARNING) << "Parsed device message command = " << static_cast<int>(dp->command) <<
				", sequence = " << mp->sequence << ", timestamp = " << mp->timestamp << ".";
		}
	}
	else
	{
		LOG(ERROR) << "Parsed error device message type = " << static_cast<int>(mp->type) << 
			", sequence = " << mp->sequence << ", timestamp = " << mp->timestamp << ".";
	}
}
