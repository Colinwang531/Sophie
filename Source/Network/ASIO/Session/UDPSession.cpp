#include "boost/bind.hpp"
#include "boost/checked_delete.hpp"
#include "Error.h"
#include "Define.h"
#include "Network/ASIO/Session/UDPSession.h"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			UDPSession::UDPSession(
				boost::asio::ip::udp::socket s,
				AfterUDPSendResultCallback scb /* = nullptr */,
				AfterUDPReceiveResultCallback rcb /* = nullptr */)
				: so{ std::move(s) }, s_callback{ scb }, r_callback{ rcb }, buffer{ nullptr }, bufsize{ 0 }
			{
				init();
			}
			UDPSession::~UDPSession()
			{
				uninit();
			}

			int UDPSession::send(
				const void* data /* = nullptr */, 
				const int bytes /* = 0 */, 
				const char* ipv4 /* = nullptr */, 
				const unsigned short port /* = 0 */)
			{
				int e{
					data && 0 < bytes && ipv4 && gMinPortNumber < port && gMaxPortNumber > port ? eSuccess : eInvalidParameter };

				if (eSuccess == e)
				{
					so.async_send_to(
						boost::asio::buffer(data, bytes), 
						boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(ipv4), port),
						[this](boost::system::error_code e, std::size_t bytes_transferred)
						{
							if (s_callback)
							{
								s_callback(e.value(), static_cast<int>(bytes_transferred));
							}
						});
				}

				return e;
			}

			int UDPSession::receive()
			{
				int e{ buffer && 0 < bufsize ? eSuccess : eInvalidParameter };

				if (eSuccess == e)
				{
					boost::asio::ip::udp::endpoint ep;
					so.async_receive_from(
						boost::asio::buffer(buffer, bufsize), 
						ep,
						[this, ep](boost::system::error_code e, std::size_t bytes_transferred)
						{
							if (r_callback)
							{
								r_callback(e.value(), buffer, static_cast<int>(bytes_transferred), ep.address().to_v4().to_string().c_str(), ep.port());
							}
						});
				}

				return e;
			}

			void UDPSession::init()
			{
				if (!buffer)
				{
					buffer = new(std::nothrow) char[gMaxBufferSize + 1] {0};
				}

				if (buffer)
				{
					bufsize = gMaxBufferSize;
				}
			}

			void UDPSession::uninit()
			{
				so.close();
				boost::checked_array_delete(buffer);
			}
		}//namespace asio
	}//namespace network
}//namespace framework
