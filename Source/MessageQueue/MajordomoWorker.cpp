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
#include "Network/AbstractClient.h"
using AbstractClient = base::network::AbstractClient;
#include "MessageQueue/MessageData.h"
using MessageData = mq::message::MessageData;
#include "MessageQueue/MajordomoWorker.h"

namespace mq
{
    namespace module
    {
		MajordomoWorker::MajordomoWorker(
			const std::string name, void* client /* = nullptr */)
			: workerName{ name }, abstractClient{ client }, dealer{ nullptr },
			workerID{ boost::uuids::to_string(boost::uuids::random_generator()()) }
		{}

		MajordomoWorker::~MajordomoWorker()
		{
			stopWorker();
		}

		int MajordomoWorker::startWorker(const std::string address)
		{
			int e{ 
				address.empty() || workerName.empty() || workerID.empty() ? eInvalidParameter : eSuccess };

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
						dealer->setsockopt(ZMQ_IDENTITY, workerID.c_str(), workerID.length());
						e = 0 < dealer->connect(address.c_str()) ? eSuccess : eBadConnect;

						if (eSuccess == e)
						{
							void* t1{
								ThreadPool::get_mutable_instance().createNewThread(
									boost::bind(&MajordomoWorker::pollerThreadProc, this)) };
							void* t2{
								ThreadPool::get_mutable_instance().createNewThread(
									boost::bind(&MajordomoWorker::autoRegisterToMajordomoBrokerThreadProc, this)) };
							e = t1 && t2 ? eSuccess : eBadNewThread;
						}
						else
						{
							stopWorker();
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

		int MajordomoWorker::stopWorker()
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

		int MajordomoWorker::sendData(
			const std::string flagID, 
			const std::string fromID, 
			const std::string toID, 
			const std::string msg)
		{
			MessageData md;
			md.pushFront(msg);
			md.pushFront(toID);
			md.pushFront(fromID);
			md.pushFront(flagID);
			md.pushFront("");
			return md.sendData(dealer);
		}

		void MajordomoWorker::pollerThreadProc()
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
						//Request/Response
						const std::string flagID{ msgdata.popFront() };
						//FromID
						const std::string fromID{ msgdata.popFront() };
						//ToID
						const std::string toID{ msgdata.popFront() };
						//Data
						const std::string data{ msgdata.popFront() };

						//数据内容
						ac->afterClientPolledMessageProcess(flagID, fromID, toID, data);
					}
				}
			}
		}

		void MajordomoWorker::autoRegisterToMajordomoBrokerThreadProc()
		{
			AbstractClient* ac{ reinterpret_cast<AbstractClient*>(abstractClient) };
			//起始时间设为0保证第一次时间判断就执行注册
			long long startTime{ Time().tickcount() };

			while (ac && !ac->isStopped())
			{
				long long currentTime{ Time().tickcount() };
				if (30000 < currentTime - startTime)
				{
					startTime = currentTime;
					//发送Worker实例注册消息
					const std::string msgstr{ ac->buildAutoRegisterToBrokerMessage() };
					if (!msgstr.empty())
					{
						MessageData msgdata;
						msgdata.pushFront("");
						msgdata.pushFront(msgstr);
						msgdata.sendData(dealer);
					}
				}

				boost::this_thread::sleep(boost::posix_time::seconds(1));
			}
		}
    }//namespace module
}//namespace mq
