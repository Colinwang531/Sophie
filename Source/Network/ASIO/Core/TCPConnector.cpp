#include "Define.h"
#include "Error.h"
#include "Network/ASIO/Core/TCPConnector.h"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			TCPConnector::TCPConnector(
				AfterTCPConnectResultCallback callback /* = nullptr */)
				: c_callback{ callback }
			{}
			TCPConnector::~TCPConnector()
			{}

			int TCPConnector::connect(
				boost::asio::ip::tcp::socket& so, 
				const char* ipv4 /* = nullptr */, 
				const unsigned short port /* = 0 */)
			{
				int e{
					ipv4 && gMaxPortNumber > port && gMinPortNumber < port ? eSuccess : eInvalidParameter };

				if (eSuccess == e)
				{
					//auto self{ boost::enable_shared_from_this<TCPConnector>::shared_from_this() };
					so.async_connect(
						boost::asio::ip::tcp::endpoint(
							boost::asio::ip::address::from_string(ipv4), port),
						[this/*, self*/](boost::system::error_code e/*, boost::asio::ip::tcp::endpoint*/)
						{
							if (c_callback)
							{
								c_callback(e.value());
							}
						});
				}

				return e;
			}
		}//namespace asio
	}//namespace network
}//namespace framework
