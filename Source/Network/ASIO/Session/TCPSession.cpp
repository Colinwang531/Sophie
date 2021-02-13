#include "Define.h"
#include "Error.h"
#include "Network/ASIO/Session/TCPSession.h"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			TCPSession::TCPSession(
				boost::asio::ip::tcp::socket& s, 
				AfterTCPSendResultCallback scb /* = nullptr */, 
				AfterTCPReceiveResultCallback rcb /* = nullptr */)
				: so{ std::move(s) }, s_callback{ scb }, r_callback{ rcb }, buffer{ nullptr }, bufsize{ 0 }
			{
				init();
			}
			TCPSession::~TCPSession()
			{
				uninit();
			}

			int TCPSession::send(
				const void* data /* = nullptr */, 
				const int bytes /* = 0 */)
			{
				int e{ data && 0 < bytes ? eSuccess : eInvalidParameter };

				if (eSuccess == e)
				{
					int pos{ 0 }, transferred{ 0 };

					while (pos < bytes)
					{
						transferred = bytes - pos;
						transferred = (transferred > gNetworkBufferMtuBytes ? gNetworkBufferMtuBytes : transferred);
						so.async_write_some(
							boost::asio::buffer(data + pos, transferred),
							[this](boost::system::error_code e, std::size_t bytes_transferred)
							{
								if (s_callback)
								{
									s_callback(e.value(), static_cast<int>(bytes_transferred));
								}
							});
						pos += transferred;
					}
				}

				return e;
			}

			int TCPSession::receive()
			{
				int e{ buffer && 0 < bufsize ? eSuccess : eInvalidParameter };

				if (eSuccess == e)
				{
					so.async_read_some(
						boost::asio::buffer(buffer, bufsize),
						[this](boost::system::error_code e, std::size_t bytes_transferred)
						{
							if (r_callback)
							{
								r_callback(e.value(), buffer, static_cast<int>(bytes_transferred));
							}
						});
				}

				return e;
			}

			void TCPSession::init()
			{
				if (!buffer)
				{
					buffer = new(std::nothrow) char[gMaxBufferSize + 1]{ 0 };
				}

				if (buffer)
				{
					bufsize = gMaxBufferSize;
				}
			}

			void TCPSession::uninit()
			{
				so.close();
				boost::checked_array_delete(buffer);
			}
		}//namespace asio
	}//namespace network
}//namespace framework
