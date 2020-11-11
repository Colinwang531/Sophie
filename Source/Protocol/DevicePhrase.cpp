#include "boost/make_shared.hpp"
#include "Error.h"
#include "Device/SurveillanceDevice.h"
using AbstractCamera = base::device::AbstractCamera;
using AbstractDevice = base::device::AbstractDevice;
using SurveillanceDevice = base::device::SurveillanceDevice;
#include "Packet/Message/MessagePacket.h"
using MessagePacket = base::packet::MessagePacket;
using MessagePacketPtr = boost::shared_ptr<MessagePacket>;
#ifdef WINDOWS
#include "Protocol/win/Message.pb.h"
#include "Protocol/win/Device.pb.h"
#else
#include "Protocol/linux/Message.pb.h"
#include "Protocol/linux/Device.pb.h"
#endif//WINDOWS
#include "Protocol/DevicePhrase.h"

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
			msg::MSG mm;
			mm.set_type(msg::MSG_Type::MSG_Type_DEVICE);
			mm.set_sequence(pkt->getPacketSequence());
			mm.set_timestamp(pkt->getPacketTimestamp());
			msg::Device* d{ mm.mutable_device() };

			if (d)
			{
				MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
				const msg::Device_Command command{
					static_cast<msg::Device_Command>(msgpkt->getMessagePacketCommand()) };
				d->set_command(command);
				
				if (msg::Device_Command::Device_Command_NEW_REP == command ||
					msg::Device_Command::Device_Command_DELETE_REP == command ||
					msg::Device_Command::Device_Command_MODIFY_REP == command)
				{
					const int status{ msgpkt->getMessageStatus() };
					msg::DeviceResponse* rep{ d->mutable_deviceresponse() };
					rep->set_result(status);
					msg::DeviceInfo* devinfo{ rep->add_deviceinfos() };
					devinfo->set_enable(true);

					if (eSuccess == status)
					{
						const std::vector<AbstractCamera>* cameras{
							reinterpret_cast<const std::vector<AbstractCamera>*>(pkt->getPacketData()) };
						if (cameras)
						{
							for (int i = 0; i != cameras->size(); ++i)
							{
								AbstractCamera ac{ (*cameras)[i] };
								msg::CameraInfo* camerainfo{ devinfo->add_camerainfos() };
								camerainfo->set_cid(ac.getCameraID());
								camerainfo->set_index(ac.getCameraIndex());
								camerainfo->set_enable(ac.getEnableFlag());
								camerainfo->set_ip(ac.getIPAddress());
								camerainfo->set_enable(ac.getEnableFlag());
							}
						}
					}

					devinfo->set_did(
						reinterpret_cast<const char*>(pkt->getPacketData(1)));
				}

				mm.SerializeToString(&msgstr);
				mm.release_component();
			}

 			return msgstr;
		}
	}//namespace protocol
}//namespace base
