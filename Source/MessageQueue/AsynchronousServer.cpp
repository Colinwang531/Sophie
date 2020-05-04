#include "boost/format.hpp"
#include "zmq.h"
#include "Define.h"
#include "Error.h"
#include "Hardware/Cpu.h"
using Cpu = hw::cpu::Cpu;
#include "MessageQueue/AsynchronousServer.h"

namespace mq
{
	namespace module
	{
		AsynchronousServer::AsynchronousServer()
			: ctx{ nullptr }, router{ nullptr }, dealer{ nullptr }, stopped{ true }
		{}

		AsynchronousServer::~AsynchronousServer()
		{}

		int AsynchronousServer::startServer(
			const unsigned short port /* = 61001 */, const char* upstreamAddress /* = nullptr */, const unsigned short upstreamPort /* = 61101 */)
		{
			int e{ 
				minPortNumber < port && maxPortNumber > port && minPortNumber < upstreamPort && maxPortNumber > upstreamPort && upstreamAddress 
				? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				//异步服务端包含消息队列上下文实例、路由端实例和分发端实例
				//路由端和分发端实例依赖于消息队列上下文实例创建
				//路由端负责监听外部连接,分发端负责处理请求
				
				ctx = zmq_init(2 * Cpu().getCPUCoreNumber());
				if (ctx)
				{
					e = createNewModule(port, (boost::format("tcp://%s:%d") % upstreamAddress % upstreamPort).str().c_str());

					if (eSuccess == e)
					{
						stopped = false;
						startWorker();
						startPoller();
					}
				}
			}

			return e;
		}

		int AsynchronousServer::stopServer()
		{
			//异步服务端销毁前先退出工作线程再销毁模型实例

			stopped = true;
			for (int i = 0; i != threadGroup.size(); ++i)
			{
				zmq_threadclose(threadGroup[i]);
			}

			return destroyModule();
		}

		int AsynchronousServer::createNewModule(const unsigned short port /* = 60000 */, const char* address /* = nullptr */)
		{
			router = zmq_socket(ctx, ZMQ_ROUTER);
			if (router)
			{
				if (zmq_bind(router, (boost::format("tcp://*:%d") % port).str().c_str()))
				{
					zmq_close(router);
					return eBindPortFail;
				}
			}

			dealer = zmq_socket(ctx, ZMQ_DEALER);
			if (dealer)
			{
				if (zmq_bind(dealer, inprocAddressID))
				{
					zmq_close(dealer);
					return eBindInprocFail;
				}
			}

			return eSuccess;
		}

		int AsynchronousServer::destroyModule()
		{
			int e{ ctx && router && dealer ? eSuccess : eBadOperate };

			if (eSuccess == e)
			{
				zmq_close(router);
				zmq_close(dealer);
				zmq_ctx_shutdown(ctx);
			}

			return e;
		}

		void AsynchronousServer::addPollItem(std::vector<void*>& items)
		{
			if (router && dealer)
			{
				items.push_back(router);
				items.push_back(dealer);
			}
		}

		void AsynchronousServer::afterPollItemMessage(void* s /* = nullptr */)
		{
			if (router == s)
			{
				transferData(router, dealer);
			}
			else if (dealer == s)
			{
				transferData(dealer, router);
			}
		}

		int AsynchronousServer::startPoller()
		{
			void* thread{ zmq_threadstart(&AsynchronousServer::pollerThreadProc, this, "poller") };

			if (thread)
			{
				threadGroup.push_back(thread);
			}

			return eSuccess;
		}

		void AsynchronousServer::pollerThreadProc(void* param /* = nullptr */)
		{
			AsynchronousServer* server{ reinterpret_cast<AsynchronousServer*>(param) };

			if (server)
			{
				std::vector<void*> items;
				server->addPollItem(items);
				const int itemNumber{ items.size() };

				if (0 < itemNumber)
				{
					zmq_pollitem_t* pollitems{ new(std::nothrow) zmq_pollitem_t[itemNumber] };

					if (pollitems)
					{
						for (int i = 0; i != itemNumber; ++i)
						{
							pollitems[i].socket = items[i];
							pollitems[i].fd = 0;
							pollitems[i].events = ZMQ_POLLIN;
							pollitems[i].revents = 0;
						}

						while (!server->stopped)
						{
							zmq_poll(pollitems, itemNumber, 1);

							for (int i = 0; i != itemNumber; ++i)
							{
								if (pollitems[i].revents & ZMQ_POLLIN)
								{
									server->afterPollItemMessage(pollitems[i].socket);
								}
							}
						}
					}
				}
			}
		}

		int AsynchronousServer::startWorker()
		{
			const int cpuCoreNumber{ 2 * Cpu().getCPUCoreNumber() };

			for (int i = 0; i != cpuCoreNumber; ++i)
			{
				void* thread{ zmq_threadstart(&AsynchronousServer::workerThreadProc, this, (boost::format("worker%d") % i).str().c_str()) };

				if (thread)
				{
					threadGroup.push_back(thread);
				}
			}

			return eSuccess;
		}

		void AsynchronousServer::workerThreadProc(void* param /* = nullptr */)
		{
			AsynchronousServer* server{ reinterpret_cast<AsynchronousServer*>(param) };
			void* worker{ zmq_socket(server->ctx, ZMQ_DEALER) };

			if (worker && !zmq_connect(worker, inprocAddressID))
			{
				while (!server->stopped)
				{
					int /*tempSendDataBytes = 0, */flag{ ZMQ_DONTWAIT };
					zmq_msg_t recvMsg;
					zmq_msg_init(&recvMsg);
					int recvDataBytes{ zmq_msg_recv(&recvMsg, worker, flag) };

					if (zmq_msg_more(&recvMsg))
					{
						flag |= ZMQ_SNDMORE;
					}

// 					zmq_msg_t sendMsg;
// 					zmq_msg_init(&sendMsg);
// 					zmq_msg_copy(&sendMsg, &recvMsg);
// 					int sendDataBytes = zmq_msg_send(&sendMsg, to, flag);

					zmq_msg_close(&recvMsg);
//					zmq_msg_close(&sendMsg);
				}
			}

			zmq_close(worker);
		}

		void AsynchronousServer::transferData(void* src /* = nullptr */, void* dest /* = nullptr */)
		{
			int /*tempSendDataBytes = 0, */flag{ ZMQ_DONTWAIT };
			zmq_msg_t msg/*recvMsg*/;
			zmq_msg_init(&msg/*recvMsg*/);
			const int recvDataBytes{ zmq_msg_recv(&msg/*recvMsg*/, src, flag) };

			if (zmq_msg_more(&msg/*recvMsg*/))
			{
				flag |= ZMQ_SNDMORE;
			}

// 			zmq_msg_t sendMsg;
// 			zmq_msg_init(&sendMsg);
// 			zmq_msg_copy(&sendMsg, &recvMsg);
			/*int sendDataBytes = */zmq_msg_send(&msg/*sendMsg*/, dest, flag);

//			zmq_msg_close(&recvMsg);
			zmq_msg_close(&msg/*sendMsg*/);
		}
	}//namespace module
}//namespace mq
