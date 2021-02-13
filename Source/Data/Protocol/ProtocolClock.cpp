#include "output/Clock.pb.h"
#include "Data/Protocol/ProtocolClock.h"

namespace framework
{
	namespace data
	{
		ProtocolClock::ProtocolClock() 
		{}
		ProtocolClock::~ProtocolClock() 
		{}

		const std::string ProtocolClock::unpack(const std::string msg)
		{
			std::string str;
			msg::ClockMessage msg_;
			if (msg_.ParseFromArray(msg.c_str(), static_cast<int>(msg.length())))
			{
				//Clock
				str = msg_.clock();
			}

			return str;
		}

		const std::string ProtocolClock::pack(const std::string str)
		{
			std::string s;
			msg::ClockMessage clkmsg;

			//Clock
			clkmsg.set_clock(str);

			clkmsg.SerializeToString(&s);
			return s;
		}
	}//namespace protocol
}//namespace base
