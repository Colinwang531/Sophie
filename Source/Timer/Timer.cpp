#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "Error.h"
#include "Timer/Timer.h"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			Timer::Timer(AfterTimerExpireResultCallback callback /* = nullptr */)
				: afterTimerExpireResultCallback{ callback }
			{}
			Timer::~Timer()
			{}

			int Timer::setTime(
				const void* s /* = nullptr */, 
				const int timeo /* = 5 */)
			{
				int e{ s && 0 < timeo ? eSuccess : eInvalidParameter };

				if (eSuccess == e)
				{
					using DeadLineTimerPtr = boost::shared_ptr<boost::asio::deadline_timer>;
					boost::asio::ip::tcp::socket* so{ 
						reinterpret_cast<boost::asio::ip::tcp::socket*>(const_cast<void*>(s)) };
					DeadLineTimerPtr timer{
						boost::make_shared<boost::asio::deadline_timer>(
							so->get_executor(), 
							boost::posix_time::seconds(timeo)) };

					if (timer)
					{
						timer->async_wait(
							boost::bind(
								&Timer::afterTimerExpireResultNotification,
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

			void Timer::afterTimerExpireResultNotification(
				const boost::system::error_code e)
			{
				if (afterTimerExpireResultCallback)
				{
					afterTimerExpireResultCallback();
				}
			}
		}//namespace asio
	}//namespace network
}//namespace framework
