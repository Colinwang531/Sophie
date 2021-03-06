#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/thread/thread.hpp"
#include "time.h"

namespace framework
{
	namespace libcommon
	{
		Time::Time()
		{}
		Time::~Time()
		{}

		const unsigned long long Time::tickcount()
		{
			boost::posix_time::ptime end{ 
				boost::posix_time::microsec_clock::local_time() };
			boost::posix_time::ptime begin{ 
				boost::posix_time::time_from_string("1970-01-01 00:00:00") };
			boost::posix_time::time_duration duration{ end - begin };

			return duration.total_milliseconds();
		}

		void Time::sleep(const unsigned int interval /* = 1 */)
		{
			boost::this_thread::sleep(boost::posix_time::seconds(interval));
		}
	}//namespace libcommon
}//namespace framework
