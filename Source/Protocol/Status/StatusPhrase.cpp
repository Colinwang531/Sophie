#include "boost/checked_delete.hpp"
#include "boost/polymorphic_cast.hpp"
#include "Packet/Message/MessagePacket.h"
using AbstractPacket = base::packet::AbstractPacket;
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

		void* StatusParser::unpackFromStatusMessage(void* s /* = nullptr */)
		{
			AbstractPacket* ap{ nullptr };
			msg::Status* ms{ reinterpret_cast<msg::Status*>(s) };

			if (ms)
			{
				const StatusCommand command{ 
					static_cast<StatusCommand>(ms->command()) };
				ap = new(std::nothrow) MessagePacket(
					base::packet::PacketType::PACKET_TYPE_STATUS, static_cast<int>(command));

				if (ap)
				{
					if (StatusCommand::STATUS_COMMAND_SET_REQ == command ||
						StatusCommand::STATUS_COMMAND_QUERY_REQ == command)
					{
						msg::StatusRequest* sr { ms->release_statusrequest() };
						const msg::StatusRequest_Type srt{ sr->type() };

						if (msg::StatusRequest_Type::StatusRequest_Type_SAIL == srt || 
							msg::StatusRequest_Type::StatusRequest_Type_AIS == srt)
						{
							int* flag{ new(std::nothrow) int };

							if (flag)
							{
								*flag = sr->flag();
								ap->setPacketData(flag);
							} 
							else
							{
								boost::checked_delete(boost::polymorphic_downcast<MessagePacket*>(ap));
								ap = nullptr;
							}
						}
					}
					else if (StatusCommand::STATUS_COMMAND_SET_REP == command ||
						StatusCommand::STATUS_COMMAND_QUERY_REP == command)
					{
					}
				}
			}

			return ap;
		}

		StatusPacker::StatusPacker(){}
		StatusPacker::~StatusPacker(){}

		void* StatusPacker::packStatus(
			const int command /* = 0 */, 
			const int result /* = 0 */, 
			void* data /* = nullptr */)
		{
			const msg::Status_Command cmd{ static_cast<msg::Status_Command>(command) };
			msg::Status* s{ msg::Status().New() };

			if (s)
			{
				s->set_command(cmd);

				if (msg::Status_Command::Status_Command_SET_REP == cmd)
				{
					msg::StatusResponse* rep{ s->mutable_statusresponse() };
					rep->set_result(result);
					if (data)
					{
						rep->set_name(reinterpret_cast<const char*>(data));
					}
				}
			}

			return s;
		}
	}//namespace protocol
}//namespace base
