#include "Error.h"
#include "Protocol/Message.pb.h"
#include "Protocol/Crew/Crew.pb.h"
#include "Protocol/Crew/CrewCodec.h"

namespace base
{
	namespace protocol
	{
		CrewCodec::CrewCodec(){}
		CrewCodec::~CrewCodec(){}

		int CrewCodec::unpackCrew(void* msg /* = nullptr */)
		{
			int e{ msg ? eSuccess : eInvalidParameter };

// 			if (eSuccess == e)
// 			{
// 				msg::MSG* m{ reinterpret_cast<msg::MSG*>(msg) };
// 
// 				if (m && m->has_component())
// 				{
// 					const msg::Component c{ m->component() };
// 				}
// 			}

			return e;
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
