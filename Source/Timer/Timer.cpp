#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "Error.h"
#include "Timer/Timer.h"

namespace base
{
	namespace network
	{
		Timer::Timer(AfterGotTimeoutNotificationCallback callback /* = nullptr */)
			: afterGotTimeoutNotificationCallback{ callback }
		{}

		Timer::~Timer()
		{}

		int Timer::waitTimeout(
			boost::asio::ip::tcp::socket* s /* = nullptr */, const int expire /* = 5 */)
		{
			int e{ s && s->is_open() && 0 < expire ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				DeadLineTimerPtr timer{
					boost::make_shared<boost::asio::deadline_timer>(
						s->get_executor(), boost::posix_time::seconds(expire)) };

				if (timer)
				{
					timer->async_wait(
						boost::bind(
							&Timer::afterWaitTimeoutNotificationCallback,
							boost::enable_shared_from_this<Timer>::shared_from_this(),
							boost::asio::placeholders::error));
				}
				else
				{
					e = eBadNewObject;
				}
			}

			return e;
		}

		void Timer::afterWaitTimeoutNotificationCallback(boost::system::error_code e)
		{
			if (!e && afterGotTimeoutNotificationCallback)
			{
				afterGotTimeoutNotificationCallback();
			}
		}
	}
}
