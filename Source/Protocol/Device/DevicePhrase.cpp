#include "boost/checked_delete.hpp"
#include "boost/make_shared.hpp"
#include "Error.h"
#include "Device/SurveillanceDevice.h"
using AbstractDevice = base::device::AbstractDevice;
using SurveillanceDevice = base::device::SurveillanceDevice;
#include "Packet/Message/MessagePacket.h"
using MessagePacket = base::packet::MessagePacket;
using MessagePacketPtr = boost::shared_ptr<MessagePacket>;
#include "Protocol/Message.pb.h"
#include "Protocol/Device/Device.pb.h"
#include "Protocol/Device/DevicePhrase.h"

namespace base
{
	namespace protocol
	{
		DeviceParser::DeviceParser(){}
		DeviceParser::~DeviceParser(){}

		DataPacketPtr DeviceParser::parseMessage(void* d /* = nullptr */)
		{
			DataPacketPtr pkt{
				boost::make_shared<MessagePacket>(
					base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_DEVICE) };

			if (pkt)
			{
				msg::Device* md{ reinterpret_cast<msg::Device*>(d) };
				const msg::Device_Command command{ md->command() };
				MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
				msgpkt->setMessagePacketCommand(static_cast<int>(command));

				if (msg::Device_Command::Device_Command_NEW_REQ == command ||
					msg::Device_Command::Device_Command_DELETE_REQ == command ||
					msg::Device_Command::Device_Command_MODIFY_REQ == command)
				{
					std::string did;
					base::device::SurveillanceDeviceFactory sdf = base::device::SurveillanceDeviceFactory::SURVEILLANCE_DEVICE_FACTORY_NONE;
					base::device::SurveillanceDeviceType sdt = base::device::SurveillanceDeviceType::SURVEILLANCE_DEVICE_TYPE_NONE;
					msg::DeviceRequest* req{ md->release_devicerequest() };

					if (req->has_deviceinfo())
					{
						const msg::DeviceInfo& info{ req->deviceinfo() };
						did = info.did();
						sdf = static_cast<base::device::SurveillanceDeviceFactory>(info.factory());
						sdt = static_cast<base::device::SurveillanceDeviceType>(info.type());
					}
					else if (req->has_did())
					{
						did = req->did();
					}
					
					SurveillanceDevice* sd{new(std::nothrow) SurveillanceDevice(did, sdf, sdt) };
					if (sd && req->has_deviceinfo())
					{
						const msg::DeviceInfo& info{ req->deviceinfo() };
						sd->setEnableFlag(info.enable());
						sd->setDeviceIPv4Address(info.ip());
						sd->setDevicePortNumber(info.port());
						sd->setLoginUserName(info.name());
						sd->setLoginUserPassword(info.password());
						pkt->setPacketData(sd);
					}
				}
				else if (msg::Device_Command::Device_Command_NEW_REP == command ||
					msg::Device_Command::Device_Command_DELETE_REP == command ||
					msg::Device_Command::Device_Command_MODIFY_REP == command)
				{
					msg::DeviceResponse* rep{ md->release_deviceresponse() };
					msgpkt->setMessageStatus(rep->result());
					if (rep->has_did())
					{
						pkt->setPacketData((void*)rep->did().c_str());
					}
					else if (0 < rep->deviceinfos_size())
					{
						for (int i = 0; i != rep->deviceinfos_size(); ++i)
						{
							const msg::DeviceInfo& info{ rep->deviceinfos(i) };
							SurveillanceDevice* sd{ 
								new(std::nothrow) SurveillanceDevice(
									info.did(), 
									static_cast<base::device::SurveillanceDeviceFactory>(info.factory()), 
									static_cast<base::device::SurveillanceDeviceType>(info.type())) };

							if (sd)
							{
								sd->setEnableFlag(info.enable());
								sd->setDeviceIPv4Address(info.ip());
								sd->setDevicePortNumber(info.port());
								sd->setLoginUserName(info.name());
								sd->setLoginUserPassword(info.password());
								pkt->setPacketData(sd);
							}
						}
					}
				}
			}

			return pkt;
		}

		DevicePacker::DevicePacker() {}
		DevicePacker::~DevicePacker() {}

		const std::string DevicePacker::packMessage(DataPacketPtr pkt)
		{
 			std::string msgstr;
// 			const base::packet::MessagePacketType msgpkttype{
// 					static_cast<base::packet::MessagePacketType>(pkt->getPacketPayloadType()) };
// 			msg::MSG mm;
// 			mm.set_type(static_cast<msg::MSG_Type>(msgpkttype));
// 			mm.set_sequence(pkt->getPacketSequence());
// 			mm.set_timestamp(pkt->getPacketTimestamp());
// 			msg::Device* d{ mm.mutable_device() };
// 
// 			if (d)
// 			{
// 				const msg::Device_Command command{
// 					static_cast<msg::Device_Command>(pkt->getPacketPayloadCommand()) };
// 				d->set_command(command);
// 				
// 				if (msg::Device_Command::Device_Command_NEW_REP == command ||
// 					msg::Device_Command::Device_Command_DELETE_REP == command ||
// 					msg::Device_Command::Device_Command_MODIFY_REP == command)
// 				{
// 					msg::DeviceResponse* rep{ d->mutable_deviceresponse() };
// 					int* result{ reinterpret_cast<int*>(pkt->getPacketExtendData()) };
// 					rep->set_result(*result);
// 				}
// 
// 				mm.SerializeToString(&msgstr);
// 				mm.release_component();
// 			}

 			return msgstr;
		}
	}//namespace protocol
}//namespace base
