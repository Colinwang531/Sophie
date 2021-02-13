#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "Define.h"
#include "Error.h"
#include "Network/ASIO/Core/UDPSender.h"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			UDPSender::UDPSender(AfterUDPSendResultCallback callback /* = nullptr */)
				: afterUDPSendResultCallback{ callback }
			{}
			UDPSender::~UDPSender()
			{}

			int UDPSender::send(
				const void* s /* = nullptr */, 
				const char* ipv4 /* = nullptr */, 
				const unsigned short port /* = 0 */, 
				const void* data /* = nullptr */, 
				const int bytes /* = 0 */)
			{
				int e{
					s && data && 0 < bytes && ipv4 && gMinPortNumber < port && gMaxPortNumber > port ? eSuccess : eInvalidParameter };

				if (eSuccess == e)
				{
					boost::asio::ip::udp::socket* so{ 
						reinterpret_cast<boost::asio::ip::udp::socket*>(const_cast<void*>(s)) };

					if (!so->is_open())
					{
						so->open(boost::asio::ip::udp::v4());
					}

					so->async_send_to(
						boost::asio::buffer(data, bytes),
						boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(ipv4), port),
						boost::bind(
							&UDPSender::afterUDPSendResultNotification,
							boost::enable_shared_from_this<UDPSender>::shared_from_this(),
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred));
				}

				return e;
			}

			void UDPSender::afterUDPSendResultNotification(
				const boost::system::error_code e, 
				const int bytes /* = 0 */)
			{
				if (afterUDPSendResultCallback)
				{
					afterUDPSendResultCallback(e.value(), bytes);
				}
			}
		}//namespace asio
	}//namespace network
}//namespace framework
