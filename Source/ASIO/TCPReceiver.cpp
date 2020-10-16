#include "boost/bind.hpp"
#include "boost/checked_delete.hpp"
#include "boost/make_shared.hpp"
#include "Timer/Timer.h"
using TimerPtr = boost::shared_ptr<base::network::Timer>;
#include "ASIO/TCPSession.h"
#include "ASIO/TCPReceiver.h"

namespace base
{
	namespace network
	{
		TCPReceiver::TCPReceiver(TCPSession* s)
			: afterGotReadDataNotificationCallback{}, session{ s }, expired{ false }, dataBuffer{ nullptr }
		{
			dataBuffer = new(std::nothrow) unsigned char[/*eMaxDataSize*/ 100 * 1024];
		}

		TCPReceiver::TCPReceiver(
			AfterGotReadDataNotificationCallback callback)
			: afterGotReadDataNotificationCallback{ callback }, session{ nullptr }, expired{ false }
		{
			dataBuffer = new(std::nothrow) unsigned char[/*eMaxDataSize*/100 * 1024];
		}

		TCPReceiver::~TCPReceiver()
		{
			boost::checked_array_delete(dataBuffer);
		}

		void TCPReceiver::recvData(
			boost::asio::ip::tcp::socket* s /* = nullptr */, 
			const int timeout /* = 0 */)
		{
			if (s && s->is_open() && dataBuffer)
			{
				s->async_read_some(
					boost::asio::buffer(dataBuffer, /*eMaxDataSize*/100 * 1024),
					boost::bind(
						&TCPReceiver::afterReadDataNotificationCallback,
						boost::enable_shared_from_this<TCPReceiver>::shared_from_this(),
						s,
						boost::asio::placeholders::bytes_transferred,
						boost::asio::placeholders::error));

				if (0 < timeout)
				{
					TimerPtr timer{
						boost::make_shared<Timer>(
							boost::bind(
								&TCPReceiver::afterReadDataExpiredNotificationCallback,
								boost::enable_shared_from_this<TCPReceiver>::shared_from_this())) };

					if (timer)
					{
						timer->waitTimeout(s, timeout);
					}
				}
			}
		}

		void TCPReceiver::afterReadDataNotificationCallback(
			boost::asio::ip::tcp::socket* s, 
			std::size_t bytes, 
			boost::system::error_code e)
		{
			if (!expired && session && session->valid())
			{
				if (e)
				{
					session->receivedExceptionNotification(e);
				}
				else
				{
					session->receivedDataNotification(dataBuffer, (int)bytes);
				}
			}

			if (!expired && afterGotReadDataNotificationCallback)
			{
				afterGotReadDataNotificationCallback(s, dataBuffer, (int)bytes);
			}
		}

		void TCPReceiver::afterReadDataExpiredNotificationCallback()
		{
			if (!expired)
			{
				expired = true;

				if (session && session->valid())
				{
					session->receivedExpiredNotification();
				}
			}
		}
	}//namespace network
}//namespace base
