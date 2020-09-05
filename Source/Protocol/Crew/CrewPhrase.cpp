#include "Protocol/Message.pb.h"
#include "Protocol/Crew/Crew.pb.h"
#include "Protocol/Crew/CrewPhrase.h"

namespace base
{
	namespace protocol
	{
		CrewParser::CrewParser(){}
		CrewParser::~CrewParser(){}

		void* CrewParser::parseCrewMessage(void* c /* = nullptr */)
		{
			return nullptr;
// 			msg::MSG* mm{ reinterpret_cast<msg::MSG*>(msg) };
// 			AbstractPacket* ap{ nullptr };
// 			msg::Crew* mc{ mm->release_crew() };
// 
// 			if (mc)
// 			{
// 				const CrewCommand command{ 
// 					static_cast<CrewCommand>(mc->command()) };
// 
// 				if (CrewCommand::CREW_COMMAND_NEW_REQ == command ||
// 					CrewCommand::CREW_COMMAND_DELETE_REQ == command || 
// 					CrewCommand::CREW_COMMAND_MODIFY_REQ == command || 
// 					CrewCommand::CREW_COMMAND_QUERY_REQ == command)
// 				{
// 					ap = new(std::nothrow) MessagePacket(
// 						base::packet::PacketType::PACKET_TYPE_CREW, static_cast<int>(command));
// 
// 					if (ap)
// 					{
// 						const msg::CrewInfo& info{ mc->release_crewrequest()->crewinfo() };
// 						AbstractPerson* person{ new(std::nothrow) Crew(info.uid()) };
// 
// 						if (person)
// 						{
// 							person->setPersonName(info.name());
// 							person->setPersonJob(info.job());
// 							for (int i = 0; i != info.pictures().size(); ++i)
// 							{
// 								person->addPersonPicture(info.pictures()[i]);
// 							}
// 							ap->setPacketData(person);
// 						}
// 						else
// 						{
// 							boost::checked_delete(boost::polymorphic_downcast<MessagePacket*>(ap));
// 							ap = nullptr;
// 						}
// 					}
// 				}
// 				else if (CrewCommand::CREW_COMMAND_NEW_REP == command ||
// 					CrewCommand::CREW_COMMAND_DELETE_REP == command ||
// 					CrewCommand::CREW_COMMAND_MODIFY_REP == command ||
// 					CrewCommand::CREW_COMMAND_QUERY_REP == command)
// 				{
// 					const int result{ mc->release_crewresponse()->result() };
// 					ap = new(std::nothrow) MessagePacket(
// 						base::packet::PacketType::PACKET_TYPE_CREW, static_cast<int>(command), result);
// 				}
// 			}
// 
// 			return ap;
		}

		CrewPacker::CrewPacker() {}
		CrewPacker::~CrewPacker() {}

		void* CrewPacker::packCrew(
			const int command /* = 0 */,
			const int result /* = 0 */,
			void* data /* = nullptr */)
		{
			const msg::Crew_Command cmd{ static_cast<msg::Crew_Command>(command) };
			msg::Crew* c{ msg::Crew().New() };

			if (c)
			{
				c->set_command(cmd);

				if (msg::Crew_Command::Crew_Command_NEW_REP == cmd || 
					msg::Crew_Command::Crew_Command_DELETE_REP == cmd || 
					msg::Crew_Command::Crew_Command_MODIFY_REP == cmd)
				{
					msg::CrewResponse* rep{ c->mutable_crewresponse() };
					rep->set_result(result);
				}
			}

			return c;
		}
	}//namespace protocol
}//namespace base
