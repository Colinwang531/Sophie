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

		void* DeviceParser::unpackFromDeviceMessage(void* d /* = nullptr */)
		{
			AbstractPacket* ap{ nullptr };
			msg::Device* md{ reinterpret_cast<msg::Device*>(d) };

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
						const msg::Embedded embedded{ md->release_devicerequest()->embedded() };
						const msg::Embedded_Factory et{ embedded.factory() };

						if (base::device::DeviceFactory::DEVICE_FACTORY_HK == static_cast<base::device::DeviceFactory>(et))
						{
							ad = new(std::nothrow) HikvisionDevice(
								embedded.did(), 
								static_cast<base::device::DeviceType>(embedded.type()));
						} 
						else
						{
						}

						if (ad)
						{
							ad->setDeviceIPv4Address(embedded.ip());
							ad->setDevicePortNumber(embedded.port());
							ad->setLoginUserName(embedded.name());
							ad->setLoginUserPassword(embedded.password());
							ap->setPacketData(ad);
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
