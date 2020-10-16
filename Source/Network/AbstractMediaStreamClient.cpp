#include "boost/bind.hpp"
#include "boost/format.hpp"
#include "boost/make_shared.hpp"
#include "Error.h"
#include "ASIO/TCPConnector.h"
using TCPConnector = base::network::TCPConnector;
using TCPConnectorPtr = boost::shared_ptr<TCPConnector>;
#include "Network/AbstractMediaStreamClient.h"

namespace base
{
	namespace network
	{
		AbstractMediaStreamClient::AbstractMediaStreamClient(
			const std::string address,
			const unsigned short port /* = 60531 */)
			: AbstractClient(base::network::ClientModuleType::CLIENT_MODULE_TYPE_MAJORDOMO_WORKER),
			mediaStreamIP{ address }, mediaStreamPort{ port }
		{}

		AbstractMediaStreamClient::~AbstractMediaStreamClient()
		{}

		int AbstractMediaStreamClient::createNewClientModule(const std::string address, const std::string name)
		{
			int e{ AbstractClient::createNewClientModule(address, name) };

			if (eSuccess == e)
			{
				e = asioService.startService();
			}

			return e;
		}

		int AbstractMediaStreamClient::destroyClientModule()
		{
			asioService.stopService();
			return AbstractClient::destroyClientModule();
		}

		int AbstractMediaStreamClient::connectMediaServerWithID(
			const std::string did, 
			const std::string cid, 
			const int idx /* = -1 */)
		{
			int e{ mediaStreamIP.empty() || cid.empty() || did.empty() || 0 > idx ? eInvalidParameter : eSuccess };

			if (eSuccess == e)
			{
				TCPConnectorPtr connector{ 
					boost::make_shared<TCPConnector>(
						asioService,
						boost::bind(&AbstractMediaStreamClient::afterGotRemoteConnectedNotificationCallback, this, _1, _2)) };
				if (connector)
				{
					urlGroup.pushBack((boost::format("%s:%d") % did % idx).str());
					e = connector->setConnect(mediaStreamIP.c_str(), mediaStreamPort);
				}
				else
				{
					e = eBadNewObject;
				}
			}

			return e;
		}

		int AbstractMediaStreamClient::disconnectMediaServer(const std::string did)
		{
			int e{ did.empty() ? eInvalidParameter : eSuccess };

			if (eSuccess == e)
			{
			}

			return e;
		}

		void AbstractMediaStreamClient::afterGotRemoteConnectedNotificationCallback(
			boost::asio::ip::tcp::socket* s, const boost::system::error_code e)
		{
			if (s && !e)
			{
				const std::string url{ urlGroup.at() };
				if (!url.empty())
				{
					urlGroup.removeFront();
					createNewMediaStreamSession(url, s);
				}
			}
		}
	}//namespace network
}//namespace base
