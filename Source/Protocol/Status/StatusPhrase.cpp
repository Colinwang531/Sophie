#include "boost/checked_delete.hpp"
#include "Packet/Message/MessagePacket.h"
using MessagePacket = base::packet::MessagePacket;
#include "Protocol/Message.pb.h"
#include "Protocol/Status/Status.pb.h"
#include "Protocol/Status/StatusPhrase.h"

namespace base
{
	namespace protocol
	{
		StatusParser::StatusParser(){}
		StatusParser::~StatusParser(){}

		void* StatusParser::parseStatusMessage(void* s /* = nullptr */)
		{
			MessagePacket* mp{ nullptr };

// 			if (s)
// 			{
// 				msg::Status* ms{ reinterpret_cast<msg::Status*>(s) };
// 				const msg::Status_Command command{ ms->command() };
// 				mp = new(std::nothrow) MessagePacket(
// 					base::packet::PacketType::PACKET_TYPE_STATUS, static_cast<int>(command));
// 
// 				if (mp)
// 				{
// 					if (msg::Status_Command::Status_Command_SET_REQ == command ||
// 						msg::Status_Command::Status_Command_QUERY_REQ == command)
// 					{
// 						msg::StatusRequest* sr{ ms->release_statusrequest() };
// 						const msg::StatusRequest_Type srt{ sr->type() };
// 
// 						if (msg::StatusRequest_Type::StatusRequest_Type_SAIL == srt ||
// 							msg::StatusRequest_Type::StatusRequest_Type_AIS == srt)
// 						{
// 							int* flag{ new(std::nothrow) int };
// 
// 							if (flag)
// 							{
// 								*flag = sr->flag();
// 								mp->setPayloadData(flag);
// 							}
// 							else
// 							{
// 								boost::checked_delete(mp);
// 								mp = nullptr;
// 							}
// 						}
// 					}
// 					else if (msg::Status_Command::Status_Command_SET_REP == command ||
// 						msg::Status_Command::Status_Command_QUERY_REP == command)
// 					{
// 					}
// 				}
// 			}

			return mp;
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
