#include "boost/bind.hpp"
#include "boost/checked_delete.hpp"
#include "boost/functional/factory.hpp"
#include "Define.h"
#include "Error.h"
#include "ASIO/ASIOService.h"
#include "ASIO/TCPConnector.h"

namespace base
{
	namespace network
	{
		TCPConnector::TCPConnector(
			ASIOService& service,
			AfterGotRemoteConnectedNotificationCallback callback /* = nullptr */)
			: afterGotRemoteConnectedNotificationCallback{ callback }, asioService{ service }
		{}

		TCPConnector::~TCPConnector()
		{}

		int TCPConnector::setConnect(const char* address /* = nullptr */, const unsigned short port /* = 60531 */)
		{
			int e{ gMaxPortNumber > port && gMinPortNumber < port ? eSuccess : eInvalidParameter };
			boost::asio::io_service& ioservice{ asioService.getIdle() };

			if (eSuccess == e && !ioservice.stopped())
			{
				boost::asio::ip::tcp::socket* s{ boost::factory<boost::asio::ip::tcp::socket*>()(ioservice) };

				if (s)
				{
					s->async_connect(
						boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(address), port),
						boost::bind(
							&TCPConnector::afterRemoteConnectedNotificationCallback,
							boost::enable_shared_from_this<TCPConnector>::shared_from_this(),
							s,
							boost::asio::placeholders::error));
				} 
				else
				{
					e = eBadNewObject;
				}
			}

			return e;
		}

		void TCPConnector::afterRemoteConnectedNotificationCallback(boost::asio::ip::tcp::socket* s, const boost::system::error_code e)
		{
			if (afterGotRemoteConnectedNotificationCallback)
			{
				afterGotRemoteConnectedNotificationCallback(s, e);
			}

			if (e.value() && s)
			{
				s->close();
				boost::checked_delete(s);
			}
		}
	}//namespace network
}//namespace base
