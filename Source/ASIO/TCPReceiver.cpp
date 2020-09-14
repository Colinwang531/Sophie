#include "ASIO/TCPReceiver.h"

NS_BEGIN(asio, 2)

TCPReceiver::TCPReceiver(AfterReadDataNotificationCallback callback /* = nullptr */)
	: afterReadDataNotificationCallback{ callback }, isReadData{ false }
{}

TCPReceiver::~TCPReceiver()
{}

void TCPReceiver::asyncRead(boost::asio::ip::tcp::socket* so /* = nullptr */, const UInt32 timeo /* = 0 */)
{
	if (so && so->is_open())
	{
		so->async_read_some(
			boost::asio::buffer(readDataBuffer, gMaxMTUBytes),
			boost::bind(
				&TCPReceiver::afterAsyncReadDataNotificationCallback,
				boost::enable_shared_from_this<TCPReceiver>::shared_from_this(),
				so,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));

//		so->async_read_some(boost::asio::buffer(receiveData, gMaxMTUBytes), callback);

		if (0 < timeo)
		{
			TimerPtr tempReadDataTimeoutPtr{
				boost::make_shared<NS(timer, 1)::Timer>(
					so, boost::bind(&TCPReceiver::afterAsyncReadDataTimeoutNotificationCallback, this, _1, _2)) };

			if (tempReadDataTimeoutPtr)
			{
				tempReadDataTimeoutPtr->asyncWait(timeo);
				readDataTimeoutPtr.swap(tempReadDataTimeoutPtr);
			}
		}
	}
}

void TCPReceiver::afterAsyncReadDataNotificationCallback(boost::system::error_code error, std::size_t transfferedBytes /* = 0 */)
{
	if (!isReadData)
	{
		isReadData = true;
	}

	if (afterReadDataNotificationCallback)
	{
		afterReadDataNotificationCallback(so, readDataBuffer, transfferedBytes, error.value());
	}
}

void TCPReceiver::afterAsyncReadDataTimeoutNotificationCallback(boost::asio::ip::tcp::socket* so /* = nullptr */, const Int32 error /* = 0 */)
{
	if (!isReadData && afterReadDataNotificationCallback)
	{
		afterReadDataNotificationCallback(so, nullptr, 0, -1);
	}
}

NS_END
