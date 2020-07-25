#include "boost/checked_delete.hpp"
#include "boost/polymorphic_cast.hpp"
#include "Device/Hikvision/HikvisionDevice.h"
using AbstractDevice = base::device::AbstractDevice;
using HikvisionDevice = base::device::HikvisionDevice;
#include "Packet/Message/MessagePacket.h"
using AbstractPacket = base::packet::AbstractPacket;
using MessagePacket = base::packet::MessagePacket;
#include "Protocol/Message.pb.h"
#include "Protocol/Device/Device.pb.h"
#include "Protocol/Device/DevicePhrase.h"

namespace base
{
	namespace protocol
	{
		DeviceParser::DeviceParser(){}
		DeviceParser::~DeviceParser(){}

		void* DeviceParser::parseDeviceMessage(void* msg /* = nullptr */)
		{
			msg::MSG* mm{ reinterpret_cast<msg::MSG*>(msg) };
			AbstractPacket* ap{ nullptr };
			msg::Device* md{ mm->release_device() };

			if (md)
			{
				const DeviceCommand command{ 
					static_cast<DeviceCommand>(md->command()) };

				if (DeviceCommand::DEVICE_COMMAND_NEW_REQ == command ||
					DeviceCommand::DEVICE_COMMAND_DELETE_REQ == command ||
					DeviceCommand::DEVICE_COMMAND_MODIFY_REQ == command)
				{
					ap = new(std::nothrow) MessagePacket(
						base::packet::PacketType::PACKET_TYPE_DEVICE, static_cast<int>(command));

					if (ap)
					{
						AbstractDevice* ad{ nullptr };
						const msg::DeviceInfo& info{ md->release_devicerequest()->deviceinfo() };
						const msg::DeviceInfo_Factory factory{ info.factory() };

						if (base::device::DeviceFactory::DEVICE_FACTORY_HK == static_cast<base::device::DeviceFactory>(factory))
						{
							ad = new(std::nothrow) HikvisionDevice(
								info.did(), 
								static_cast<base::device::DeviceType>(info.type()));
						} 
						else
						{
						}

						if (ad)
						{
							ad->setDeviceIPv4Address(info.ip());
							ad->setDevicePortNumber(info.port());
							ad->setLoginUserName(info.name());
							ad->setLoginUserPassword(info.password());
							ap->setPacketData(ad);
							ap->setPacketSequence(mm->sequence());
						}
						else
						{
							boost::checked_delete(boost::polymorphic_downcast<MessagePacket*>(ap));
							ap = nullptr;
						}
					}
				}
				else if (DeviceCommand::DEVICE_COMMAND_NEW_REP == command ||
					DeviceCommand::DEVICE_COMMAND_DELETE_REP == command ||
					DeviceCommand::DEVICE_COMMAND_MODIFY_REP == command)
				{

				}
			}

			return ap;
		}
	}//namespace protocol
}//namespace base
