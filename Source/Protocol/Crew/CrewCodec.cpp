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
	}//namespace protocol
}//namespace base
