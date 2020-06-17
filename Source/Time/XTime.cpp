#include "boost/date_time/posix_time/posix_time.hpp"
#include "Time/XTime.h"

namespace base
{
	namespace time
	{
		Time::Time(){}
		Time::~Time(){}

		const long long Time::tickcount()
		{
			boost::posix_time::ptime end{ 
				boost::posix_time::microsec_clock::local_time() };
			boost::posix_time::ptime begin{ 
				boost::posix_time::time_from_string("1970-01-01 00:00:00") };
			boost::posix_time::time_duration duration{ end - begin };

			return duration.total_milliseconds();
		}
	}//namespace time
}//namespace base
