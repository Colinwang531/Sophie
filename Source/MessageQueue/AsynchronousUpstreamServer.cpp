#include "boost/format.hpp"
#include "zmq.h"
#include "Define.h"
#include "Error.h"
#include "MessageQueue/AsynchronousUpstreamServer.h"

namespace mq
{
	namespace module
	{
		AsynchronousUpstreamServer::AsynchronousUpstreamServer() : AsynchronousServer(), dealer{ nullptr }
		{}

		AsynchronousUpstreamServer::~AsynchronousUpstreamServer()
		{}

		int AsynchronousUpstreamServer::createNewModule(const unsigned short port /* = 60000 */, const char* address /* = nullptr */)
		{
			int e{ AsynchronousServer::createNewModule(port) };

			if (eSuccess == e)
			{
				//创建上游服务端模型
				dealer = zmq_socket(ctx, ZMQ_DEALER);
				if (dealer)
				{
					if (0 != zmq_connect(dealer, address))
					{
						e = eBadConnect;
					}
				}
				else
				{
					e = eBadNewSocket;
				}
			}

			return e;
		}

		int AsynchronousUpstreamServer::destroyModule()
		{
			int e{ dealer ? eSuccess : eBadOperate };

			if (eSuccess == e)
			{
				zmq_close(dealer);
			}

			return e == eSuccess ? AsynchronousServer::destroyModule() : e;
		}

		void AsynchronousUpstreamServer::addPollItem(std::vector<void*>& items)
		{
			if (dealer)
			{
				items.push_back(dealer);
			}

			AsynchronousServer::addPollItem(items);
		}

		void AsynchronousUpstreamServer::afterPollItemMessage(void* s /* = nullptr */)
		{
			if (dealer == s)
			{
			}
			else
			{
				AsynchronousServer::afterPollItemMessage(s);
			}
		}
	}//namespace module
}//namespace mq
