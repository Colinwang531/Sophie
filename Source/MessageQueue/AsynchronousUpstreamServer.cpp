#include "boost/format.hpp"
#include "zmq.h"
#include "Define.h"
#include "Error.h"
#include "Thread/ThreadPool.h"
using ThreadPool = base::thread::ThreadPool;
#include "MessageQueue/AsynchronousUpstreamServer.h"

namespace mq
{
	namespace module
	{
		AsynchronousUpstreamServer::AsynchronousUpstreamServer() : AsynchronousServer(), dealer{ nullptr }
		{}

		AsynchronousUpstreamServer::~AsynchronousUpstreamServer()
		{}

		int AsynchronousUpstreamServer::createNewModule(
			const unsigned short listenPort /* = 61001 */, 
			const char* upstreamIP /* = nullptr */, 
			const unsigned short upstreamPort /* = 61101 */)
		{
			int e{ AsynchronousServer::createNewModule(listenPort) };

			if (eSuccess == e && upstreamIP && gMinPortNumber < upstreamPort && gMaxPortNumber > upstreamPort)
			{
				dealer = zmq_socket(ctx, ZMQ_DEALER);
				if (dealer)
				{
					if (0 != zmq_connect(dealer, (boost::format("tcp://%s:%d") % upstreamIP % upstreamPort).str().c_str()))
					{
						e = eBadConnect;
					}
				}
				else
				{
					e = eBadNewSocket;
				}
			}
			else
			{
				e = eInvalidParameter;
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

		int AsynchronousUpstreamServer::startPoller()
		{
			int e{ AsynchronousServer::startPoller() };

			if (eSuccess == e)
			{
				void* t{ 
					ThreadPool::get_mutable_instance().createNewThread(
						boost::bind(&AsynchronousUpstreamServer::pollerThreadProc, this)) };
				e = t ? eSuccess : eBadNewThread;
			}

			return e;
		}

		void AsynchronousUpstreamServer::pollerThreadProc()
		{
			zmq_pollitem_t pollitems[] = { { dealer, 0, ZMQ_POLLIN, 0 } };

			while (!stopped)
			{
				zmq_poll(pollitems, 1, 1);
				if (pollitems[0].revents & ZMQ_POLLIN)
				{
				}
			}
		}
	}//namespace module
}//namespace mq
