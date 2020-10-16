#include "boost/checked_delete.hpp"
#include "boost/make_shared.hpp"
#include "Error.h"
#include "Packet/Message/MessagePacket.h"
using MessagePacket = base::packet::MessagePacket;
using MessagePacketPtr = boost::shared_ptr<MessagePacket>;
#include "Person/Crew/Crew.h"
using AbstractPerson = base::person::AbstractPerson;
using Crew = base::person::Crew;
#ifdef WINDOWS
#include "Protocol/win/Message.pb.h"
#include "Protocol/win/Crew.pb.h"
#else
#include "Protocol/linux/Message.pb.h"
#include "Protocol/linux/Crew.pb.h"
#endif//WINDOWS
#include "Protocol/CrewPhrase.h"

namespace base
{
	namespace protocol
	{
		CrewParser::CrewParser(){}
		CrewParser::~CrewParser(){}

		DataPacketPtr CrewParser::parseMessage(void* c /* = nullptr */)
		{
			DataPacketPtr pkt{
				boost::make_shared<MessagePacket>(
					base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_CREW) };

			if (pkt)
			{
				msg::Crew* mc{ reinterpret_cast<msg::Crew*>(c) };
				const msg::Crew_Command command{ mc->command() };
				MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
				msgpkt->setMessagePacketCommand(static_cast<int>(command));

				if (msg::Crew_Command::Crew_Command_NEW_REQ == command ||
					msg::Crew_Command::Crew_Command_DELETE_REQ == command ||
					msg::Crew_Command::Crew_Command_MODIFY_REQ == command)
				{
					const msg::CrewInfo& info{
							mc->release_crewrequest()->crewinfo() };
					AbstractPerson* person{ new(std::nothrow) Crew(info.uid()) };

					if (person)
					{
						person->setPersonName(info.name());
						person->setPersonJob(info.job());
						for (int i = 0; i != info.pictures().size(); ++i)
						{
							person->addPersonPicture(info.pictures()[i]);
						}
						pkt->setPacketData(person);
					}
				}
// 				else if (CrewCommand::CREW_COMMAND_NEW_REP == command ||
// 					CrewCommand::CREW_COMMAND_DELETE_REP == command ||
// 					CrewCommand::CREW_COMMAND_MODIFY_REP == command ||
// 					CrewCommand::CREW_COMMAND_QUERY_REP == command)
// 				{
// 					const int result{ mc->release_crewresponse()->result() };
// 					ap = new(std::nothrow) MessagePacket(
// 						base::packet::PacketType::PACKET_TYPE_CREW, static_cast<int>(command), result);
// 				}
			}

			return pkt;
		}

		CrewPacker::CrewPacker() {}
		CrewPacker::~CrewPacker() {}

		const std::string CrewPacker::packMessage(DataPacketPtr pkt)
		{
			std::string msgstr;
			msg::MSG mm;
			mm.set_type(msg::MSG_Type::MSG_Type_CREW);
			mm.set_sequence(pkt->getPacketSequence());
			mm.set_timestamp(pkt->getPacketTimestamp());
			msg::Crew* c{ mm.mutable_crew() };

			if (c)
			{
				MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
				const msg::Crew_Command command{
					static_cast<msg::Crew_Command>(msgpkt->getMessagePacketCommand()) };
				c->set_command(command);

				if (msg::Crew_Command::Crew_Command_NEW_REP == command ||
					msg::Crew_Command::Crew_Command_DELETE_REP == command ||
					msg::Crew_Command::Crew_Command_MODIFY_REP == command)
				{
					msg::CrewResponse* rep{ c->mutable_crewresponse() };
					rep->set_result(msgpkt->getMessageStatus());
				}
			}

			mm.SerializeToString(&msgstr);
			mm.release_component();
			return msgstr;
		}
	}//namespace protocol
}//namespace base
