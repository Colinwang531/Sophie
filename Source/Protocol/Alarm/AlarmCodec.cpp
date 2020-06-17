#include "Error.h"
#include "Protocol/Message.pb.h"
#include "Protocol/Alarm/Alarm.pb.h"
#include "Protocol/Alarm/AlarmCodec.h"

namespace base
{
	namespace protocol
	{
		AlarmCodec::AlarmCodec(){}
		AlarmCodec::~AlarmCodec(){}

		int AlarmCodec::unpackAlarm(void* msg /* = nullptr */)
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
