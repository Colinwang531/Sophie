#include "boost/bind.hpp"
#include "boost/checked_delete.hpp"
#include "boost/functional/factory.hpp"
#include "boost/make_shared.hpp"
#include "Define.h"
#include "Error.h"
#include "ASIO/ASIOService.h"
#include "ASIO/TCPListener.h"

namespace base
{
	namespace network
	{
		TCPListener::TCPListener(
			ASIOService& service, AfterGotRemoteConnectedNotificationCallback callback /* = nullptr */)
			: afterGotRemoteConnectedNotificationCallback{ callback }, asioService{ service }
		{}

		TCPListener::~TCPListener()
		{}

		int TCPListener::setListen(const unsigned short port /* = 60531 */)
		{
			int e{ gMaxPortNumber > port && gMinPortNumber < port ? eSuccess : eInvalidParameter };
			boost::asio::io_service& ioservice{ asioService.getIdle() };

			if (eSuccess == e && !ioservice.stopped())
			{
				boost::asio::ip::tcp::socket* s{ 
					boost::factory<boost::asio::ip::tcp::socket*>()(ioservice) };
				boost::shared_ptr<boost::asio::ip::tcp::acceptor> ap{
					boost::make_shared<boost::asio::ip::tcp::acceptor>(
						ioservice, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) };

				if (s && ap)
				{
					//After the acceptor has been initialized, listen() is called to make the acceptor start listening.
					//Then async_accept() is called to accept the first connection attempt.
					//A socket has to be passed as a first parameter to async_accept(), which will be used to send and receive data on a new connection.
					ap->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
					ap->listen();
					ap->async_accept(*s,
						boost::bind(
							&TCPListener::afterRemoteConnectedtNotificationCallback,
							boost::enable_shared_from_this<TCPListener>::shared_from_this(),
							s,
							ap,
							boost::asio::placeholders::error));
				}
				else
				{
					boost::checked_delete(s);
					e = eBadNewObject;
				}
			}

			return e;
		}

		void TCPListener::afterRemoteConnectedtNotificationCallback(
			boost::asio::ip::tcp::socket* s,
			boost::shared_ptr<boost::asio::ip::tcp::acceptor> ap,
			boost::system::error_code e)
		{
			if (!e)
			{
				boost::asio::ip::tcp::socket* so{
					boost::factory<boost::asio::ip::tcp::socket*>()(asioService.getIdle()) };

				if (so)
				{
					ap->async_accept(*so,
						boost::bind(
							&TCPListener::afterRemoteConnectedtNotificationCallback,
							boost::enable_shared_from_this<TCPListener>::shared_from_this(),
							so,
							ap,
							boost::asio::placeholders::error));
				}
			}

			if (afterGotRemoteConnectedNotificationCallback)
			{
				afterGotRemoteConnectedNotificationCallback(s, e);
			}
		}
	}//namespace network
}//namespace base
