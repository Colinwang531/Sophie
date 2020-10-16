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
#include "Protocol/win/Event.pb.h"
#else
#include "Protocol/linux/Message.pb.h"
#include "Protocol/linux/Event.pb.h"
#endif//WINDOWS
#include "Protocol/EventPhrase.h"

namespace base
{
	namespace protocol
	{
		EventParser::EventParser() {}
		EventParser::~EventParser() {}

		DataPacketPtr EventParser::parseMessage(void* e /* = nullptr */)
		{
			DataPacketPtr pkt{
				boost::make_shared<MessagePacket>(
					base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_EVENT) };

			if (pkt)
			{
				msg::Event* me{ reinterpret_cast<msg::Event*>(e) };
				const msg::Event_Command command{ me->command() };
				MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
				msgpkt->setMessagePacketCommand(static_cast<int>(command));

				if (msg::Event_Command::Event_Command_CAPTURE_JPEG_REQ == command)
				{
					msg::CaptureInfo* info{ me->release_captureinfo() };
					const std::string did{ info->did() }, cid{ info->cid() };
					const int cidx{ info->idx() }, didLen{ (int)did.length() }, cidLen{ (int)cid.length() };

					char* deviceID{ new(std::nothrow) char[didLen] };
					char* cameraID{ new(std::nothrow) char[cidLen] };
					int* cameraIdx{ new(std::nothrow) int };

					if (deviceID && cameraID && cameraIdx)
					{
#ifdef WINDOWS
						memcpy_s(deviceID, didLen, did.c_str(), didLen);
						memcpy_s(cameraID, cidLen, cid.c_str(), cidLen);
#else
						memcpy(deviceID, did.c_str(), didLen);
						memcpy(cameraID, cid.c_str(), cidLen);
#endif//WINDOWS
						*cameraIdx = cidx;

						pkt->setPacketData(deviceID);
						pkt->setPacketData(cameraID);
						pkt->setPacketData(cameraIdx);
					} 
					else
					{
						boost::checked_array_delete(deviceID);
						boost::checked_array_delete(cameraID);
						boost::checked_delete(cameraIdx);
					}
				}
			}

			return pkt;
		}

		EventPacker::EventPacker(){}
		EventPacker::~EventPacker(){}

		const std::string EventPacker::packMessage(DataPacketPtr pkt)
		{
			std::string msgstr;
			msg::MSG mm;
			mm.set_type(msg::MSG_Type::MSG_Type_EVENT);
			mm.set_sequence(pkt->getPacketSequence());
			mm.set_timestamp(pkt->getPacketTimestamp());
			msg::Event* e{ mm.mutable_evt() };

			if (e)
			{
				MessagePacketPtr msgpkt{ 
					boost::dynamic_pointer_cast<MessagePacket>(pkt) };
				const msg::Event_Command command{
					static_cast<msg::Event_Command>(msgpkt->getMessagePacketCommand()) };
				e->set_command(command);

				if (msg::Event_Command::Event_Command_CAPTURE_JPEG_REP == command && 
					eSuccess == msgpkt->getMessageStatus())
				{
					msg::CaptureInfo* info{ e->mutable_captureinfo() };
					info->set_did((const char*)pkt->getPacketData());
					info->set_cid((const char*)pkt->getPacketData(1));
					info->set_idx(*((int*)pkt->getPacketData(2)));
					info->set_picture((const char*)pkt->getPacketData(3));
				}

				mm.SerializeToString(&msgstr);
				mm.release_component();
			}

			return msgstr;
		}
	}//namespace protocol
}//namespace base
