#include "boost/checked_delete.hpp"
#include "boost/make_shared.hpp"
#include "Packet/Message/MessagePacket.h"
using MessagePacket = base::packet::MessagePacket;
using MessagePacketPtr = boost::shared_ptr<MessagePacket>;
#ifdef WINDOWS
#include "Protocol/win/Message.pb.h"
#include "Protocol/win/Status.pb.h"
#else
#include "Protocol/linux/Message.pb.h"
#include "Protocol/linux/Status.pb.h"
#endif//WINDOWS
#include "Protocol/StatusPhrase.h"

namespace base
{
	namespace protocol
	{
		StatusParser::StatusParser(){}
		StatusParser::~StatusParser(){}

		DataPacketPtr StatusParser::parseMessage(void* s /* = nullptr */)
		{
			DataPacketPtr pkt{
				boost::make_shared<MessagePacket>(
					base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_STATUS) };

			if (s)
			{
				msg::Status* ms{ reinterpret_cast<msg::Status*>(s) };
				const msg::Status_Command command{ ms->command() };
				MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
				msgpkt->setMessagePacketCommand(static_cast<int>(command));

				if (msg::Status_Command::Status_Command_SET_REQ == command)
				{
					msg::StatusRequest* sr{ ms->release_statusrequest() };
					const msg::StatusRequest_Type srt{ sr->type() };

					if (msg::StatusRequest_Type::StatusRequest_Type_SAIL == srt)
					{
						msgpkt->setMessageStatus(sr->flag());
					}
				}
			}

			return pkt;
		}

		StatusPacker::StatusPacker(){}
		StatusPacker::~StatusPacker(){}

		void* StatusPacker::packToStatusMessage(void* pkt /* = nullptr */)
		{
			MessagePacket* mp{ reinterpret_cast<MessagePacket*>(pkt) };
			msg::Status* s{ msg::Status().New() };

// 			if (s)
// 			{
// 				const msg::Status_Command command{ 
// 					static_cast<msg::Status_Command>(mp->getPayloadCommand()) };
// 				s->set_command(command);
// 
// 				if (msg::Status_Command::Status_Command_SET_REP == command)
// 				{
// 					msg::StatusResponse* rep{ s->mutable_statusresponse() };
// 					rep->set_result(mp->getReplyResult());
// 				}
// 			}

			return s;
		}
	}//namespace protocol
}//namespace base
