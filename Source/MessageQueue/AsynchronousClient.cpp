#include "boost/format.hpp"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "zmq.h"
#include "Define.h"
#include "Error.h"
#include "Hardware/Cpu.h"
using Cpu = base::hardware::Cpu;
#include "Thread/ThreadPool.h"
using ThreadPool = base::thread::ThreadPool;
#include "MessageQueue/AsynchronousClient.h"

namespace mq
{
	namespace module
	{
		AsynchronousClient::AsynchronousClient() : ctx{ nullptr }, stopped{ true }
		{}

		AsynchronousClient::~AsynchronousClient()
		{}

		int AsynchronousClient::startClient(
			const char* serverIP /* = nullptr */, 
			const unsigned short serverPort /* = 61001 */)
		{
			int e{
				serverIP && gMinPortNumber < serverPort && gMaxPortNumber > serverPort ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				ctx = zmq_init(2 * Cpu().getCPUCoreNumber());
				if (ctx)
				{
					e = createNewModule(serverIP, serverPort);

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
			stopped = true;
			ThreadPool::get_mutable_instance().shutdownAllThread();
			return destroyModule();
		}

		int AsynchronousClient::createNewModule(
			const char* serverIP /* = nullptr */, 
			const unsigned short serverPort /* = 61001 */)
		{
			int e{ eBadNewSocket };
			dealer = zmq_socket(ctx, ZMQ_DEALER);
			//每个客户端都使用ID来标识唯一的消息链路
			const std::string commID{ boost::uuids::to_string(boost::uuids::random_generator()()) };

			if (dealer && !commID.empty())
			{
				e = zmq_setsockopt(dealer, ZMQ_IDENTITY, commID.c_str(), commID.length());
				if (!e)
				{
					e = !zmq_connect(dealer, (boost::format("tcp://%s:%d") % serverIP % serverPort).str().c_str()) ? eSuccess : eBadConnect;
				}
				else
				{
					e = eBadNewObject;
				}
			}

			if (eSuccess != e)
			{
				zmq_close(dealer);
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

		int AsynchronousClient::startPoller()
		{
			void* t{
				ThreadPool::get_mutable_instance().createNewThread(
					boost::bind(&AsynchronousClient::pollerThreadProc, this)) };
			return t ? eSuccess : eBadNewThread;
		}

		void AsynchronousClient::pollerThreadProc()
		{
			int rbytes{ 0 };
			zmq_pollitem_t pollitems[] = { { dealer, 0, ZMQ_POLLIN, 0 } };

			while (!stopped)
			{
				zmq_poll(pollitems, 1, 1);
				if (pollitems[0].revents & ZMQ_POLLIN)
				{
					zmq_msg_t msg;
					zmq_msg_init(&msg);
					rbytes = zmq_msg_recv(&msg, dealer, ZMQ_DONTWAIT);
					//保证数据内容部分能被完整接收
					while (0 < zmq_msg_more(&msg))
					{
						rbytes = zmq_msg_recv(&msg, dealer, ZMQ_DONTWAIT | ZMQ_RCVMORE);
					}
					//数据处理通知
					afterClientPollMessage(reinterpret_cast<const char*>(zmq_msg_data(&msg)), rbytes);
					zmq_msg_close(&msg);
				}
			}
		}
	}//namespace module
}//namespace mq
