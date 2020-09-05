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
#include "Xml/XmlCodec.h"
using XMLParser = base::xml::XMLParser;
using XMLPacker = base::xml::XMLPacker;
#include "Component/AbstractComponent.h"
#include "Device/Hikvision/HikvisionDevice.h"
using AbstractDevice = base::device::AbstractDevice;
using HikvisionDevice = base::device::HikvisionDevice;
#include "HKDComponentClient.h"

HKDComponentClient::HKDComponentClient() : DeviceComponentClient() {}
HKDComponentClient::~HKDComponentClient() {}

const std::string HKDComponentClient::getClientID() const
{
	std::string value;
	XMLParser().getValueByName("Config.xml", "Component.HKD.ID", value);
	return value;
}

int HKDComponentClient::setClientID(const std::string ID) const
{
	return XMLPacker().setValueWithName("Config.xml", "Component.HKD.ID", ID);
}

const std::string HKDComponentClient::getClientName() const
{
	std::string value;
	XMLParser().getValueByName("Config.xml", "Component.HKD.Name", value);
	return value;
}

int HKDComponentClient::setClientName(const std::string name) const
{
	return XMLPacker().setValueWithName("Config.xml", "Component.HKD.Name", name);
}

int HKDComponentClient::getClientType() const
{
	return static_cast<int>(base::component::ComponentType::COMPONENT_TYPE_HKD);
}

void HKDComponentClient::unpackMessageFailure()
{
	LOG(ERROR) << "Unpack message failed.";
}

void HKDComponentClient::parseUnknownMessage()
{
	LOG(WARNING) << "Parsed a unknown message.";
}

int HKDComponentClient::afterParseDeviceCommandNewRequest(
	const std::string did,
	const std::string address,
	const unsigned short port,
	const std::string name,
	const std::string password)
{
	int e{
		!did.empty() && !address.empty() && 
		gMaxPortNumber > port && gMinPortNumber < port && 
		!name.empty() && !password.empty()
		? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
// 		AbstractDevicePtr devicePtr{
// 			boost::make_shared<HikvisionDevice>(name, password, address, port) };
// 		if (devicePtr)
// 		{
// 			if (eSuccess == devicePtr->createNewDevice())
// 			{
// 				abstractDeviceGroup.insert(did, devicePtr);
// 				LOG(INFO) <<
// 					"Create and log in new device successfully" <<
// 					", ID = " << did <<
// 					", IP = " << address <<
// 					", Port = " << port <<
// 					", Name = " << name <<
// 					", Password = " << password << ".";
// 			}
// 			else
// 			{
// 				e = eBadLoginDevice;
// 				LOG(WARNING) <<
// 					"Create and log in new device failed, result = " << e <<
// 					", ID = " << did <<
// 					", IP = " << address <<
// 					", Port = " << port <<
// 					", Name = " << name <<
// 					", Password = " << password << ".";
// 			}
// 		}
// 		else
// 		{
// 			e = eBadNewObject;
// 			LOG(ERROR) <<
// 				"Bad create new device object, result = " << e <<
// 				", ID = " << did <<
// 				", IP = " << address <<
// 				", Port = " << port <<
// 				", Name = " << name <<
// 				", Password = " << password << ".";
// 		}
	}

	return e;
}

int HKDComponentClient::afterParseDeviceCommandDeleteRequest(const std::string did)
{
	int e{ did.empty() ? eInvalidParameter : eSuccess };

	if (eSuccess == e)
	{
		boost::shared_ptr<AbstractDevice> adp{ abstractDeviceGroup.at(did) };
		if (adp)
		{
//			e = adp->destoryDevice();
			abstractDeviceGroup.remove(did);
			LOG(INFO) << "Delete device result = " << e << " with empty device ID = " << did << ".";
		}
		else
		{
			e = eObjectNotExist;
			LOG(WARNING) << "Can not find device ID = " << did << " to delete.";
		}
	}
	else
	{
		LOG(ERROR) << "Delete device with empty device ID.";
	}

	return e;
}
