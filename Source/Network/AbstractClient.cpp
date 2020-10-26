#include "Network/AbstractClient.h"

namespace base
{
	namespace network
	{
		AbstractClient::AbstractClient()
			: stopped{ false }
		{}

		AbstractClient::~AbstractClient()
		{}

		int AbstractClient::startClient(const std::string address)
		{
			return createNewClient(address);
		}

		int AbstractClient::stopClient()
		{
			stopped = true;
			return destroyClient();
		}
	}//namespace network
}//namespace base
