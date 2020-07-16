#include "boost/format.hpp"
#include "zmq.h"
#include "Define.h"
#include "Error.h"
#include "Hardware/Cpu.h"
using Cpu = base::hardware::Cpu;
#include "Thread/ThreadPool.h"
using ThreadPool = base::thread::ThreadPool;
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
			const unsigned short listenPort /* = 61001 */, 
			const char* upstreamIP /* = nullptr */, 
			const unsigned short upstreamPort /* = 61101 */)
		{
			int e{ 
				gMinPortNumber < listenPort && gMaxPortNumber > listenPort ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				ctx = zmq_init(2 * Cpu().getCPUCoreNumber());
				if (ctx)
				{
					e = createNewModule(listenPort, upstreamIP, upstreamPort);

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
			stopped = true;
			ThreadPool::get_mutable_instance().shutdownAllThread();
			return destroyModule();
		}

		int AsynchronousServer::createNewModule(
			const unsigned short listenPort /* = 61001 */, 
			const char* /* = nullptr */, 
			const unsigned short /* = 0 */)
		{
			router = zmq_socket(ctx, ZMQ_ROUTER);
			if (router)
			{
				if (zmq_bind(router, (boost::format("tcp://*:%d") % listenPort).str().c_str()))
				{
					zmq_close(router);
					return eBindPortFail;
				}
			}

			dealer = zmq_socket(ctx, ZMQ_DEALER);
			if (dealer)
			{
				if (zmq_bind(dealer, gInprocCommID))
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

		int AsynchronousServer::startPoller()
		{
			void* t{ 
				ThreadPool::get_mutable_instance().createNewThread(
					boost::bind(&AsynchronousServer::pollerThreadProc, this)) };
			return t ? eSuccess : eBadNewThread;
		}

		void AsynchronousServer::pollerThreadProc()
		{
			zmq_pollitem_t pollitems[] = {
					{ router, 0, ZMQ_POLLIN, 0 }, { dealer, 0, ZMQ_POLLIN, 0 } };

			while (!stopped)
			{
				zmq_poll(pollitems, 2, 1);
				if (pollitems[0].revents & ZMQ_POLLIN)
				{
					transferData(router, dealer);
				}
				else if (pollitems[1].revents & ZMQ_POLLIN)
				{
					transferData(dealer, router);
				}
			}
		}

		int AsynchronousServer::startWorker()
		{
			const int cpuCoreNumber{ 2 * Cpu().getCPUCoreNumber() };

			for (int i = 0; i != cpuCoreNumber; ++i)
			{
				ThreadPool::get_mutable_instance().createNewThread(
					boost::bind(&AsynchronousServer::workerThreadProc, this));
			}

			return eSuccess;
		}

		void AsynchronousServer::workerThreadProc()
		{
			void* worker{ zmq_socket(ctx, ZMQ_DEALER) };

			if (worker && !zmq_connect(worker, gInprocCommID))
			{
				zmq_pollitem_t pollitems[] = { { worker, 0, ZMQ_POLLIN, 0 } };

				while (!stopped)
				{
					//ROUTER-DEALER模式下的数据必须由Envelope+Delimiter+Data的格式构成,
					//所以同时接收完3个部分的数据才算一次接收结束

					zmq_poll(pollitems, 1, 1);

					if (pollitems[0].revents & ZMQ_POLLIN)
					{
						int flag{ ZMQ_DONTWAIT }, idbytes{ 0 }, delimiterbytes{ 0 }, databytes{ 0 };
						zmq_msg_t id, delimiter, data;
						zmq_msg_init(&id);
						zmq_msg_init(&delimiter);
						zmq_msg_init(&data);

						//id和delimiter部分的数据小,一次性就能接收完成
						idbytes = zmq_msg_recv(&id, worker, ZMQ_DONTWAIT);
						delimiterbytes = zmq_msg_recv(&delimiter, worker, ZMQ_DONTWAIT);
						databytes = zmq_msg_recv(&data, worker, ZMQ_DONTWAIT);
						//保证数据内容部分能被完整接收
						while (0 < zmq_msg_more(&data))
						{
							databytes = zmq_msg_recv(&data, worker, ZMQ_DONTWAIT | ZMQ_RCVMORE);
						}
						//数据处理通知
						afterServerPollMessage(
							worker,
							zmq_msg_data(&id), idbytes,
							zmq_msg_data(&delimiter), delimiterbytes,
							zmq_msg_data(&data), databytes);

						zmq_msg_close(&id);
						zmq_msg_close(&delimiter);
						zmq_msg_close(&data);
					}
				}
			}

			zmq_close(worker);
		}

		void AsynchronousServer::transferData(void* src /* = nullptr */, void* dest /* = nullptr */)
		{
			//ROUTER-DEALER模式下的数据交换是有什么就立即交换,数据由WORKER端负责处理,
			//所以消息的接收和发送不是接收一个完整的包再交换的方式

			int flag{ ZMQ_DONTWAIT };
			zmq_msg_t msg;
			zmq_msg_init(&msg);
			const int recvDataBytes{ zmq_msg_recv(&msg, src, flag) };

			if (zmq_msg_more(&msg))
			{
				flag |= ZMQ_SNDMORE;
			}
			
			zmq_msg_send(&msg, dest, flag);
			zmq_msg_close(&msg);
		}
	}//namespace module
}//namespace mq
