#include "boost/checked_delete.hpp"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "Define.h"
#include "Error.h"
#include "Xml/XmlCodec.h"
using XMLParser = base::xml::XMLParser;
#include "MessageQueue/MajordomoWorker.h"
using MajordomoWorker = mq::module::MajordomoWorker;
#include "Network/AbstractUpstreamServer.h"

namespace base
{
	namespace network
	{
		AbstractUpstreamServer::AbstractUpstreamServer(
			const ServerModuleType server/* = ServerModuleType::SERVER_MODULE_TYPE_NONE*/,
			const ClientModuleType upstream/* = ClientModuleType::CLIENT_MODULE_TYPE_NONE*/,
			const std::string address/* = ""*/)
			: AbstractServer(server), AbstractClient(upstream), upstreamAddress{ address }
		{}

		AbstractUpstreamServer::~AbstractUpstreamServer()
		{}

		int AbstractUpstreamServer::createNewServerModule(const std::string address)
		{
			int e{ AbstractServer::createNewServerModule(address) };

			if (eSuccess == e)
			{
				std::string serviceName;
				XMLParser().getValueByName("Config.xml", "Component.XMQ.Name", serviceName);
				startClient(upstreamAddress, serviceName);
			}

			return e;
		}

		int AbstractUpstreamServer::destroyServerModule()
		{
			int e{ AbstractServer::destroyServerModule() };

			if (eSuccess == e)
			{
				e = stopClient();
			}

			return e;
		}
	}//namespace network
}//namespace base
