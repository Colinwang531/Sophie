#include "boost/checked_delete.hpp"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "Define.h"
#include "Error.h"
#include "MessageQueue/MajordomoWorker.h"
using MajordomoWorker = mq::module::MajordomoWorker;
#include "Network/AbstractClient.h"

namespace base
{
	namespace network
	{
		AbstractClient::AbstractClient(
			const ClientModuleType type /* = ClientModuleType::CLIENT_MODULE_TYPE_NONE */)
			: clientModuleType{ type }, stopped{ false }, clientModule{ nullptr }
		{}

		AbstractClient::~AbstractClient()
		{}

		int AbstractClient::startClient(const std::string address, const std::string name)
		{
			return !address.empty() && !name.empty() ? createNewClientModule(address, name) : eInvalidParameter;
		}

		int AbstractClient::stopClient()
		{
			stopped = true;
			return destroyClientModule();
		}

		int AbstractClient::createNewClientModule(const std::string address, const std::string name)
		{
			if (ClientModuleType::CLIENT_MODULE_TYPE_MAJORDOMO_WORKER == clientModuleType)
			{
				MajordomoWorker* mw{ 
					new(std::nothrow) MajordomoWorker(name, this) };
				if (mw && eSuccess == mw->startWorker(address))
				{
					clientModule = mw;
				}
				else
				{
					boost::checked_delete(mw);
				}
			}
			else if (ClientModuleType::CLIENT_MODULE_TYPE_MAJORDOMO_CLIENT == clientModuleType)
			{
			}

			return clientModule ? eSuccess : eBadNewObject;
		}

		int AbstractClient::destroyClientModule()
		{
			if (ClientModuleType::CLIENT_MODULE_TYPE_MAJORDOMO_WORKER == clientModuleType)
			{
				MajordomoWorker* mw{ reinterpret_cast<MajordomoWorker*>(clientModule) };
				if (mw)
				{
					mw->stopWorker();
					boost::checked_delete(mw);
					clientModule = nullptr;
				}
			}
			else if (ClientModuleType::CLIENT_MODULE_TYPE_MAJORDOMO_CLIENT == clientModuleType)
			{
			}

			return clientModule ? eBadOperate : eSuccess;
		}

		int AbstractClient::sendMessageData(
			const std::string flagID, 
			const std::string fromID, 
			const std::string toID, 
			const std::string msg)
		{
			int e{ !msg.empty() ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				if (ClientModuleType::CLIENT_MODULE_TYPE_MAJORDOMO_WORKER == clientModuleType)
				{
					MajordomoWorker* mw{ reinterpret_cast<MajordomoWorker*>(clientModule) };
					if (mw)
					{
						e = mw->sendData(flagID, fromID, toID, msg);
					}
				}
				else if (ClientModuleType::CLIENT_MODULE_TYPE_MAJORDOMO_CLIENT == clientModuleType)
				{
				}
			}

			return e;
		}
	}//namespace network
}//namespace base
