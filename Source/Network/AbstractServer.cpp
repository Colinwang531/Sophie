#include "boost/bind.hpp"
#include "boost/checked_delete.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/format.hpp"
#include "boost/thread.hpp"
#include "Define.h"
#include "Error.h"
#include "Time/XTime.h"
using base::time::Time;
#include "Thread/ThreadPool.h"
using ThreadPool = base::thread::ThreadPool;
#include "MessageQueue/MajordomoBroker.h"
using MajordomoBroker = mq::module::MajordomoBroker;
#include "Network/AbstractServer.h"

namespace base
{
	namespace network
	{
		AbstractServer::AbstractServer(
			const ServerModuleType server /* = ServerModuleType::SERVER_MODULE_TYPE_NONE */)
			: serverModuleType{ server }, serverModule{ nullptr }, stopped{ false }
		{}

		AbstractServer::~AbstractServer()
		{}

		int AbstractServer::startServer(const unsigned short listenPort /* = 61001 */)
		{
			return gMinPortNumber < listenPort && gMaxPortNumber > listenPort ? 
				createNewServerModule((boost::format("tcp://*:%d") % listenPort).str()) : eInvalidParameter;
		}

		int AbstractServer::stopServer()
		{
			stopped = true;
			return destroyServerModule();
		}

		int AbstractServer::createNewServerModule(
			const std::string address /* = "tcp://*:61001" */)
		{
			if (ServerModuleType::SERVER_MODULE_TYPE_MAJORDOMO_BROKER == serverModuleType)
			{
				MajordomoBroker* mb{ 
					new(std::nothrow) MajordomoBroker(this) };
				if (mb && eSuccess == mb->startBroker(address))
				{
					serverModule = mb;
				} 
				else
				{
					boost::checked_delete(mb);
				}
			}
			else if (ServerModuleType::SERVER_MODULE_TYPE_ASYNC_DISPATCHER == serverModuleType)
			{
			}
			else if (ServerModuleType::SERVER_MODULE_TYPE_ASIO == serverModuleType)
			{
			}

			if (serverModule)
			{
				//创建服务端心跳业务检测线程
				//服务端在Broker模式下由Broker自己创建线程
// 				ThreadPool::get_mutable_instance().createNewThread(
// 					boost::bind(&AbstractServer::autoCheckClientTimeoutThreadProc, this));
			}

			return serverModule ? eSuccess : eBadNewObject;
		}

		int AbstractServer::destroyServerModule()
		{
			if (ServerModuleType::SERVER_MODULE_TYPE_MAJORDOMO_BROKER == serverModuleType)
			{
				MajordomoBroker* mb{ reinterpret_cast<MajordomoBroker*>(serverModule) };
				if (mb)
				{
					mb->stopBroker();
					boost::checked_delete(mb);
					serverModule = nullptr;
				}
			}
			else if (ServerModuleType::SERVER_MODULE_TYPE_ASYNC_DISPATCHER == serverModuleType)
			{
			}
			else if (ServerModuleType::SERVER_MODULE_TYPE_ASIO == serverModuleType)
			{
			}

			return serverModule ? eBadOperate : eSuccess;
		}

		int AbstractServer::sendMessageData(
			const std::string commID, 
			const std::string flagID, 
			const std::string fromID, 
			const std::string toID, 
			const std::string msg)
		{
			int e{ !commID.empty() && !flagID.empty() && !msg.empty() ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				if (ServerModuleType::SERVER_MODULE_TYPE_MAJORDOMO_BROKER == serverModuleType)
				{
					MajordomoBroker* mb{ reinterpret_cast<MajordomoBroker*>(serverModule) };
					if (mb)
					{
						e = mb->sendData(commID, flagID, fromID, toID, msg);
					}
				}
				else if (ServerModuleType::SERVER_MODULE_TYPE_ASYNC_DISPATCHER == serverModuleType)
				{
				}
				else if (ServerModuleType::SERVER_MODULE_TYPE_ASIO == serverModuleType)
				{
				}
			}

			return e;
		}

	}//namespace network
}//namespace base
