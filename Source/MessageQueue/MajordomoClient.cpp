#include "boost/bind.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/thread.hpp"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "Error.h"
#include "Hardware/Cpu.h"
using CPU = base::hardware::Cpu;
#include "Time/XTime.h"
using Time = base::time::Time;
#include "Thread/ThreadPool.h"
using ThreadPool = base::thread::ThreadPool;
#include "MessageQueue/MessageData.h"
using MessageData = mq::message::MessageData;
#include "MessageQueue/MajordomoClient.h"
#include "Network/AbstractClient.h"
using AbstractClient = base::network::AbstractClient;

namespace mq
{
    namespace module
    {
		MajordomoClient::MajordomoClient(void* client /* = nullptr */)
			: abstractClient{ client }, dealer{ nullptr },
			clientID{ boost::uuids::to_string(boost::uuids::random_generator()()) }
		{}

		MajordomoClient::~MajordomoClient()
		{
			stopClient();
		}

		int MajordomoClient::startClient(const std::string address)
		{
			int e{ 
				address.empty() || clientID.empty() ? eInvalidParameter : eSuccess };

			if (eSuccess == e && ctx.valid())
			{
				if (!dealer)
				{
					const int cores{ CPU().getCPUCoreNumber() };
					ctx.set(ZMQ_IO_THREADS, &cores, sizeof(const int));
					dealer = ctx.create_socket(ZMQ_DEALER);

					if (dealer)
					{
						int linger{ 0 };
						dealer->setsockopt(ZMQ_LINGER, &linger, sizeof(int));
						dealer->setsockopt(ZMQ_IDENTITY, clientID.c_str(), clientID.length());
						e = 0 < dealer->connect(address.c_str()) ? eSuccess : eBadConnect;

						if (eSuccess == e)
						{
							void* t{
								ThreadPool::get_mutable_instance().createNewThread(
									boost::bind(&MajordomoClient::pollerThreadProc, this)) };
							e = t ? eSuccess : eBadNewThread;
						}
						else
						{
							stopClient();
						}
					}
					else
					{
						e = eBadNewSocket;
					}
				}
				else
				{
					e = eObjectExisted;
				}
			}
			else
			{
				e = eBadOperate;
			}

			return e;
		}

		int MajordomoClient::stopClient()
		{
			int e{ dealer && ctx.valid() ? eSuccess : eBadOperate };

			if (eSuccess == e)
			{
				ctx.destroy_socket(dealer);
				ctx.shutdown();
				dealer = nullptr;
			}

			return e;
		}

		void MajordomoClient::pollerThreadProc()
		{
			AbstractClient* ac{ reinterpret_cast<AbstractClient*>(abstractClient) };
			zmq_pollitem_t socketItems[] = {
					{ static_cast<void*>(dealer), 0, ZMQ_POLLIN, 0} };

			while (ac && !ac->isStopped())
			{
				zmq_poll(socketItems, 1, 1);
				if (socketItems[0].revents & ZMQ_POLLIN)
				{
					MessageData msgdata;
					if (eSuccess == msgdata.recvData(dealer))
					{
						//去掉数据缓存中的分隔符
						msgdata.popDataFromFront();
						//数据内容
						ac->afterPolledMessageProcess(msgdata.popDataFromFront());
					}
				}
			}
		}
    }//namespace module
}//namespace mq
