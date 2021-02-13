#include "boost/bind.hpp"
#include "boost/functional/factory.hpp"
#include "boost/make_shared.hpp"
#include "Define.h"
#include "Error.h"
#include "Network/ASIO/Core/TCPListener.h"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			TCPListener::TCPListener(
				boost::asio::io_context& io,
				const unsigned short port /* = 0 */, 
				AfterTCPAcceptResultCallback callback /* = nullptr */)
				: acceptor{ io, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port) }, a_callbck{ callback }
			{}
			TCPListener::~TCPListener()
			{}

			int TCPListener::listen()
			{
				auto self{ 
					boost::enable_shared_from_this<TCPListener>::shared_from_this() };
				acceptor.async_accept(
					[this, self](boost::system::error_code e, boost::asio::ip::tcp::socket so)
					{
						if (a_callbck)
						{
							a_callbck(e.value(), so);
						}

						//没有错误就持续监听
						if (!e)
						{
							listen();
						}
					});

				return eSuccess;
			}
		}//namespace asio
	}//namespace network
}//namespace framework
