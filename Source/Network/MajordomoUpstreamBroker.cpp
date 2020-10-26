#include "boost/make_shared.hpp"
#include "boost/bind.hpp"
#include "Define.h"
#include "Error.h"
#include "Xml/XmlCodec.h"
using XMLParser = base::xml::XMLParser;
#include "Network/MajordomoUpstreamBroker.h"

namespace base
{
	namespace network
	{
		MajordomoUpstreamBroker::MajordomoUpstreamBroker()
			: AbstractServer(), AbstractWorker()
		{}

		MajordomoUpstreamBroker::~MajordomoUpstreamBroker()
		{}

		int MajordomoUpstreamBroker::createNewServer(const std::string address)
		{
			int e{ !address.empty() ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				MajordomoBrokerPtr mdbp{ 
					boost::make_shared<MajordomoBroker>(
						boost::bind(&MajordomoUpstreamBroker::afterPolledDataFromServerCallback, this, _1, _2, _3, _4, _5, _6)) };
				if (mdbp && eSuccess == mdbp->startBroker(address))
				{
					broker.swap(mdbp);
				}
				else
				{
					e = eBadNewObject;
				}
			}

			return e;
		}

		int MajordomoUpstreamBroker::destroyServer()
		{
			return broker ? broker->stopBroker() : eBadOperate;
		}

		int MajordomoUpstreamBroker::createNewClient(std::string address)
		{
			int e{ !address.empty() ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				MajordomoWorkerPtr mdwp{
					boost::make_shared<MajordomoWorker>(
						boost::bind(&MajordomoUpstreamBroker::afterPolledDataFromWorkerCallback, this, _1, _2, _3, _4, _5)) };
				if (mdwp && eSuccess == mdwp->startWorker(address))
				{
					worker.swap(mdwp);
					//在客户端注册或心跳之前创建UUID标识
					AbstractWorker::generateUUIDWithName("Dispatcher");
					e = AbstractWorker::createNewClient("");
				}
				else
				{
					e = eBadNewObject;
				}
			}

			return e;
		}

		int MajordomoUpstreamBroker::destroyClient()
		{
			return worker ? worker->stopWorker() : eBadOperate;
		}

		int MajordomoUpstreamBroker::sendData(
			const std::string roleID, 
			const std::string flagID, 
			const std::string fromID, 
			const std::string toID, 
			const std::string data)
		{
			return worker ? worker->sendData(roleID, flagID, fromID, toID, data) : eBadOperate;
		}

		int MajordomoUpstreamBroker::sendData(
			const std::string commID, 
			const std::string roleID, 
			const std::string flagID, 
			const std::string fromID, 
			const std::string toID, 
			const std::string data)
		{
			return broker->sendData(commID, roleID, flagID, fromID, toID, data);
		}

		void MajordomoUpstreamBroker::afterPolledDataFromServerCallback(
			const std::string commID, 
			const std::string roleID, 
			const std::string flagID, 
			const std::string fromID, 
			const std::string toID, 
			const std::string data)
		{}

		void MajordomoUpstreamBroker::afterPolledDataFromWorkerCallback(
			const std::string roleID, 
			const std::string flagID, 
			const std::string fromID, 
			const std::string toID, 
			const std::string data)
		{}
	}//namespace network
}//namespace base
