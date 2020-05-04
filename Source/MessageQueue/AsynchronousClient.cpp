#include "boost/format.hpp"
#include "zmq.h"
#include "Define.h"
#include "Error.h"
#include "Hardware/Cpu.h"
using Cpu = hw::cpu::Cpu;
#include "MessageQueue/AsynchronousClient.h"

namespace mq
{
	namespace module
	{
		AsynchronousClient::AsynchronousClient() : ctx{ nullptr }, stopped{ true }, workerThread{ nullptr }
		{}

		AsynchronousClient::~AsynchronousClient()
		{}

		int AsynchronousClient::startClient(const char* address /* = nullptr */, const unsigned short port /* = 61001 */)
		{
			int e{
				minPortNumber < port&& maxPortNumber > port ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				//异步服务端包含消息队列上下文实例和分发端实例
				//分发端实例依赖于消息队列上下文实例创建
				//分发端负责连接外部服务端

				ctx = zmq_init(2 * Cpu().getCPUCoreNumber());
				if (ctx)
				{
					e = createNewModule((boost::format("tcp://%s:%d") % address % port).str().c_str());

					if (eSuccess == e)
					{
						stopped = false;
						startPoller();
					}
				}
			}

			return e;
		}

		int AsynchronousClient::stopClient()
		{
			//异步客户端销毁前先退出工作线程再销毁模型实例

			stopped = true;
			zmq_threadclose(workerThread);

			return destroyModule();
		}

		int AsynchronousClient::createNewModule(const char* address /* = nullptr */)
		{
			dealer = zmq_socket(ctx, ZMQ_DEALER);
			if (dealer)
			{
				if (0 != zmq_connect(dealer, address))
				{
					zmq_close(dealer);
					return eBadConnect;
				}
			}

			return eSuccess;
		}

		int AsynchronousClient::destroyModule()
		{
			int e{ ctx && dealer ? eSuccess : eBadOperate };

			if (eSuccess == e)
			{
				zmq_close(dealer);
				zmq_ctx_shutdown(ctx);
			}

			return e;
		}

		void AsynchronousClient::addPollItem(std::vector<void*>& items)
		{
			if (dealer)
			{
				items.push_back(dealer);
			}
		}

		void AsynchronousClient::afterPollItemMessage(void* s /* = nullptr */)
		{
			if (dealer == s)
			{
			}
		}

		int AsynchronousClient::startPoller()
		{
			workerThread = zmq_threadstart(&AsynchronousClient::pollerThreadProc, this, "poller");
			return workerThread ? eSuccess : eBadNewThread;
		}

		void AsynchronousClient::pollerThreadProc(void* param /* = nullptr */)
		{
			AsynchronousClient* client{ reinterpret_cast<AsynchronousClient*>(param) };

			if (client)
			{
				std::vector<void*> items;
				client->addPollItem(items);
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

						while (!client->stopped)
						{
							zmq_poll(pollitems, itemNumber, 1);

							for (int i = 0; i != itemNumber; ++i)
							{
								if (pollitems[i].revents & ZMQ_POLLIN)
								{
									client->afterPollItemMessage(pollitems[i].socket);
								}
							}
						}
					}
				}
			}
		}

// 		int RequesterModel::send(const char* data, const int dataBytes, std::string& resp)
// 		{
// 			int status{ ERR_INVALID_PARAM };
// 
// 			if (data && 0 < dataBytes)
// 			{
// 				status = requester ? MQSender().send(data, dataBytes, requester) : ERR_BAD_OPERATE;
// 
// 				//if (ERR_OK == status)
// 				//{
// 				//	zmq_pollitem_t pollitems[] = { { requester, NULL, ZMQ_POLLIN, NULL } };
// 				//	zmq_poll(pollitems, 1, 30000);
// 				//	if (pollitems[0].revents & ZMQ_POLLIN)
// 				//	{
// 				//		status = MQReceiver().receive(requester, resp);
// 				//	}
// 				//	else
// 				//	{
// 				//		status = ERR_BAD_OPERATE;
// 				//	}
// 				//}
// 			}
// 
// 			return status;
// 		}
	}//namespace module
}//namespace mq
