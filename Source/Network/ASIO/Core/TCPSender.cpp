#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "Define.h"
#include "Error.h"
#include "Network/ASIO/Core/TCPSender.h"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			TCPSender::TCPSender(
				AfterTCPSendResultCallback callbak /* = nullptr */) : afterTCPSendResultCallback{ callbak }
			{}
			TCPSender::~TCPSender()
			{}

			int TCPSender::send(
				const void* s /* = nullptr */, 
				const char* data /* = nullptr */, 
				const int bytes /* = 0 */)
			{
				int e{ s && data && 0 < bytes ? eSuccess : eInvalidParameter };

				if (eSuccess == e)
				{
					boost::asio::ip::tcp::socket* so{ 
						reinterpret_cast<boost::asio::ip::tcp::socket*>(const_cast<void*>(s)) };
					int pos{ 0 }, currentSendBytes{ 0 };

					if (!so->is_open())
					{
						so->open(boost::asio::ip::tcp::v4());
					}

					while (pos < bytes)
					{
						currentSendBytes = bytes - pos;
						currentSendBytes = (currentSendBytes > gNetworkBufferMtuBytes ? gNetworkBufferMtuBytes : currentSendBytes);
						so->async_write_some(
							boost::asio::buffer(data + pos, currentSendBytes),
							boost::bind(
								&TCPSender::afterTCPSendResultNotification,
								boost::enable_shared_from_this<TCPSender>::shared_from_this(),
								boost::asio::placeholders::error,
								boost::asio::placeholders::bytes_transferred));
						pos += currentSendBytes;
					}
				}

				return e;
			}

			void TCPSender::afterTCPSendResultNotification(
				const boost::system::error_code e, 
				const int bytes /* = 0 */)
			{
				if (afterTCPSendResultCallback)
				{
					afterTCPSendResultCallback(e.value(), bytes);
				}
			}
		}//namespace asio
	}//namespace network
}//namespace framework
