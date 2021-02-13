#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/checked_delete.hpp"
#include "boost/make_shared.hpp"
#include "Define.h"
#include "Error.h"
#include "Network/ASIO/Core/TCPReceiver.h"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			TCPReceiver::TCPReceiver(
				AfterTCPReceiveResultCallback callback /* = nullptr */,
				const int bufsize /* = 0 */)
				: afterTCPReceiveResultCallback{ callback }, buffer{ nullptr }, bufferSize{ 0 < bufsize ? bufsize : gNetworkBufferMtuBytes }
			{
				buffer = new(std::nothrow) char[bufferSize];
			}
			TCPReceiver::~TCPReceiver()
			{
				boost::checked_array_delete(buffer);
			}

			int TCPReceiver::receive(const void* s /* = nullptr */)
			{
				int e{ s ? eSuccess : eInvalidParameter };

				if (eSuccess == e)
				{
					if (buffer)
					{
						boost::asio::ip::tcp::socket* so{ 
							reinterpret_cast<boost::asio::ip::tcp::socket*>(const_cast<void*>(s)) };
						so->async_read_some(
							boost::asio::buffer(buffer, bufferSize),
							boost::bind(
								&TCPReceiver::afterTCPReceiveResultNotification,
								boost::enable_shared_from_this<TCPReceiver>::shared_from_this(),
								boost::asio::placeholders::error,
								s,
								boost::asio::placeholders::bytes_transferred));
					}
					else
					{
						e = eBadNewMemory;
					}
				}
			}

			void TCPReceiver::afterTCPReceiveResultNotification(
				const boost::system::error_code e, 
				const void* s /* = nullptr */, 
				const int bytes /* = 0 */)
			{
				if (afterTCPReceiveResultCallback)
				{
					afterTCPReceiveResultCallback(e.value(), s, !e ? (const char*)buffer : nullptr, bytes);
				}

				//先处理数据再继续接收
				//避免数据乱序
				if (!e)
				{
					receive(s);
				}
			}
		}//namespace asio
	}//namespace network
}//namespace framework
