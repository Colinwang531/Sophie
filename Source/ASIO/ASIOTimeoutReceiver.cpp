#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "ASIO/ASIOTimeoutReceiver.h"

NS_BEGIN(asio, 1)

ASIOTimeoutReceiver::ASIOTimeoutReceiver(ReceivedCallback callback /* = nullptr */, Handle ctx /* = nullptr */, const UInt16 expire /* = 5 */)
	: ASIOReceiver(callback, ctx), secondsOfExpired{ expire }
{}

ASIOTimeoutReceiver::~ASIOTimeoutReceiver()
{}

int ASIOTimeoutReceiver::posting(Socket s /* = nullptr */)
{
	int result = 0 < secondsOfExpired ? ASIOReceiver::posting(s) : ERR_OUT_RANGE;

	if (ERR_OK == result)
	{
		boost::posix_time::seconds tempOfExpired{ secondsOfExpired };
		TCPSocket* so{ reinterpret_cast<TCPSocket*>(s) };
		TimerPtr expiredTimer{ boost::make_shared<ExpiredTimer>(so->get_io_service(), tempOfExpired) };
		expiredTimer->async_wait(boost::bind(&ASIOTimeoutReceiver::expired, this, boost::asio::placeholders::error, so, expiredTimer));
	}

	return result;
}

void ASIOTimeoutReceiver::expired(ErrorCode e, Socket s, TimerPtr timer)
{
	//第一次超时后就关闭定时器
	timer->cancel();

	if (receivedCallback)
	{
		receivedCallback(ERR_TIMER_EXPIRED, s, "", cbContext);
	}
}

NS_END
