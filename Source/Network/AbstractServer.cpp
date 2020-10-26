#include "boost/bind.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/format.hpp"
#include "Define.h"
#include "Error.h"
#include "Time/XTime.h"
using base::time::Time;
#include "Thread/ThreadPool.h"
using ThreadPool = base::thread::ThreadPool;
#include "Network/AbstractServer.h"

namespace base
{
	namespace network
	{
		AbstractServer::AbstractServer()
			: stopped{ false }
		{}

		AbstractServer::~AbstractServer()
		{}

		int AbstractServer::startServer(const unsigned short listenPort /* = 61001 */)
		{
			int e{ createNewServer((boost::format("tcp://*:%d") % listenPort).str().c_str()) };

			if (eSuccess == e)
			{
				e = ThreadPool::get_mutable_instance().createNewThread(
					boost::bind(&AbstractServer::autoCheckClientConnectionTimeoutThread, this)) ? eSuccess : eBadNewThread;
			}

			return e;
		}

		int AbstractServer::stopServer()
		{
			stopped = true;
			return destroyServer();
		}

// 		int AbstractServer::createNewServerModule(
// 			const std::string address /* = "tcp://*:61001" */)
// 		{
// 			if (ServerModuleType::SERVER_MODULE_TYPE_MAJORDOMO_BROKER == serverModuleType)
// 			{
// 				MajordomoBroker* mb{ 
// 					new(std::nothrow) MajordomoBroker(this) };
// 				if (mb && eSuccess == mb->startBroker(address))
// 				{
// 					serverModule = mb;
// 				} 
// 				else
// 				{
// 					boost::checked_delete(mb);
// 				}
// 			}
// 			else if (ServerModuleType::SERVER_MODULE_TYPE_ASYNC_DISPATCHER == serverModuleType)
// 			{
// 			}
// 			else if (ServerModuleType::SERVER_MODULE_TYPE_ASIO == serverModuleType)
// 			{
// 			}
// 
// 			if (serverModule)
// 			{
// 				//创建服务端心跳业务检测线程
// 				//服务端在Broker模式下由Broker自己创建线程
// // 				ThreadPool::get_mutable_instance().createNewThread(
// // 					boost::bind(&AbstractServer::autoCheckClientTimeoutThreadProc, this));
// 			}
// 
// 			return serverModule ? eSuccess : eBadNewObject;
// 		}
// 
// 		int AbstractServer::destroyServerModule()
// 		{
// 			if (ServerModuleType::SERVER_MODULE_TYPE_MAJORDOMO_BROKER == serverModuleType)
// 			{
// 				MajordomoBroker* mb{ reinterpret_cast<MajordomoBroker*>(serverModule) };
// 				if (mb)
// 				{
// 					mb->stopBroker();
// 					boost::checked_delete(mb);
// 					serverModule = nullptr;
// 				}
// 			}
// 			else if (ServerModuleType::SERVER_MODULE_TYPE_ASYNC_DISPATCHER == serverModuleType)
// 			{
// 			}
// 			else if (ServerModuleType::SERVER_MODULE_TYPE_ASIO == serverModuleType)
// 			{
// 			}
// 
// 			return serverModule ? eBadOperate : eSuccess;
// 		}

// 		int AbstractServer::sendMessageData(
// 			const std::string commID, 
// 			const std::string flagID, 
// 			const std::string fromID, 
// 			const std::string toID, 
// 			const std::string msg)
// 		{
// 			int e{ !commID.empty() && !flagID.empty() && !msg.empty() ? eSuccess : eInvalidParameter };
// 
// 			if (eSuccess == e)
// 			{
// 				if (ServerModuleType::SERVER_MODULE_TYPE_MAJORDOMO_BROKER == serverModuleType)
// 				{
// 					MajordomoBroker* mb{ reinterpret_cast<MajordomoBroker*>(serverModule) };
// 					if (mb)
// 					{
// 						e = mb->sendData(commID, flagID, fromID, toID, msg);
// 					}
// 				}
// 				else if (ServerModuleType::SERVER_MODULE_TYPE_ASYNC_DISPATCHER == serverModuleType)
// 				{
// 				}
// 				else if (ServerModuleType::SERVER_MODULE_TYPE_ASIO == serverModuleType)
// 				{
// 				}
// 			}
// 
// 			return e;
// 		}

		void AbstractServer::autoCheckClientConnectionTimeoutThread()
		{
			//超时时间90秒
			long long startTime{ Time().tickcount() };

			while (!isStopped())
			{
				long long nowTime{ Time().tickcount() };

				//每30秒检测一次
				if (30000 < nowTime - startTime)
				{
					afterAutoClientConnectionTimeoutProcess();
					startTime = nowTime;
				}

				boost::this_thread::sleep(boost::posix_time::seconds(1));
			}
		}
	}//namespace network
}//namespace base
