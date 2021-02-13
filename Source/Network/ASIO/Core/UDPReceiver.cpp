#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "Define.h"
#include "Error.h"
#include "Network/ASIO/Core/UDPReceiver.h"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			UDPReceiver::UDPReceiver(
				AfterUDPReceiveResultCallback callback /* = nullptr */, 
				const int bufsize /* = 0 */)
				: afterUDPReceiveResultCallback{ callback }, buffer{ nullptr }, bufferSize{ 0 < bufsize ? bufsize : gNetworkBufferMtuBytes }
			{
				buffer = new(std::nothrow) unsigned char[bufferSize];
			}
			UDPReceiver::~UDPReceiver()
			{
				boost::checked_array_delete(buffer);
			}

			int UDPReceiver::receive(const void* s /* = nullptr */)
			{
				int e{ s ? eSuccess : eInvalidParameter };

				if (eSuccess == e)
				{
					if (buffer)
					{
						boost::asio::ip::udp::socket* so{
							reinterpret_cast<boost::asio::ip::udp::socket*>(const_cast<void*>(s)) };

						so->async_receive_from(
							boost::asio::buffer(buffer, bufferSize),
							rep,
							boost::bind(
								&UDPReceiver::afterUDPReceiveResultNotification,
								boost::enable_shared_from_this<UDPReceiver>::shared_from_this(),
								boost::asio::placeholders::error,
								s,
								boost::asio::placeholders::bytes_transferred));
					} 
					else
					{
						e = eBadNewMemory;
					}
				}

				return e;
			}

			void UDPReceiver::afterUDPReceiveResultNotification(
				const boost::system::error_code e, 
				const void* s /* = nullptr */, 
				const int bytes /* = 0 */)
			{
				if (afterUDPReceiveResultCallback)
				{
					afterUDPReceiveResultCallback(
						const_cast<void*>(s), e.value(), buffer, bytes, rep.address().to_v4().to_string().c_str(), rep.port());
				}
			}
		}//namespace asio
	}//namespace network
}//namespace framework
