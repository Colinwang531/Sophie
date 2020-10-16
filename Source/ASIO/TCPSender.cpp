#include "boost/bind.hpp"
#include "boost/checked_delete.hpp"
#include "Error.h"
#include "ASIO/TCPSession.h"
#include "ASIO/TCPSender.h"

namespace base
{
	namespace network
	{
		TCPSender::TCPSender(TCPSession* s /* = nullptr */)
			: session{ s }, dataBuffer{ nullptr }, dataBufferBytes{ 0 }, sentDataBytes{ 0 }
		{
			dataBuffer = new(std::nothrow) unsigned char[3 * 1024 * 1024];
		}

		TCPSender::~TCPSender()
		{
			boost::checked_array_delete(dataBuffer);
		}

		int TCPSender::sendData(
			boost::asio::ip::tcp::socket* s /* = nullptr */, 
			const unsigned char* data /* = nullptr */, 
			const int bytes /* = 0 */)
		{
			int e{ s && s->is_open() && data && 0 < bytes ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
// 				if (0 == dataBufferBytes)
// 				{
// 					memcpy(dataBuffer, data, bytes);
// 					dataBufferBytes = bytes;
// 					sentDataBytes = 0;
// 				}

// 				boost::asio::async_write(
// 					*s, 
// 					boost::asio::buffer(data, bytes), 
// 					boost::bind(
// 						&TCPSender::afterSentDataNotificationCallback,
// 						boost::enable_shared_from_this<TCPSender>::shared_from_this(),
// 						s, 
// 						boost::asio::placeholders::bytes_transferred, 
// 						boost::asio::placeholders::error));

				const int mtu{ 100 * 1024 };
				int pos{ 0 }, sentBytes{ 0 };

				while (pos < bytes)
				{
					int leftBytes = bytes - pos;
					sentBytes = (leftBytes > mtu ? mtu : leftBytes);
					s->async_write_some(
						boost::asio::buffer(data + pos, sentBytes/*bytes*/),
						boost::bind(
							&TCPSender::afterSentDataNotificationCallback,
							boost::enable_shared_from_this<TCPSender>::shared_from_this(),
							s,
							boost::asio::placeholders::bytes_transferred,
							boost::asio::placeholders::error));
					pos += sentBytes;
				}
			}
			
			return e;
		}

		void TCPSender::afterSentDataNotificationCallback(
			boost::asio::ip::tcp::socket* s, 
			std::size_t bytes, 
			boost::system::error_code e)
		{
// 			if (!e && bytes < dataBufferBytes)
// 			{
// 				dataBufferBytes -= bytes;
// 				sentDataBytes += bytes;
// 				sendData(s, dataBuffer + sentDataBytes, dataBufferBytes);
// 			}
// 			else if (!e && session && session->valid())
// 			{
// 				dataBufferBytes = 0;
// 				sentDataBytes = 0;
// 				session->sentdDataNotification(e);
// 			}

			if (!e && session && session->valid())
			{
				session->sentdDataNotification(e);
			}
		}
	}//namespace network
}//namespace base
