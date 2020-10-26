#include "boost/bind.hpp"
//#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "Error.h"
#include "Hardware/Cpu.h"
using CPU = base::hardware::Cpu;
#include "Thread/ThreadPool.h"
using ThreadPool = base::thread::ThreadPool;
#include "MessageQueue/MessageData.h"
using MessageData = mq::message::MessageData;
#include "MessageQueue/MajordomoWorker.h"

namespace mq
{
    namespace module
    {
		MajordomoWorker::MajordomoWorker(
			AfterClientRecievedDataCallback callback /* = nullptr */)
			: afterClientRecievedDataCallback{ callback }, dealer{ nullptr }, stopped{ false }
		{}

		MajordomoWorker::~MajordomoWorker()
		{
			stopWorker();
		}

		int MajordomoWorker::startWorker(const std::string address)
		{
			int e{ ctx.valid() ? eSuccess : eBadOperate };

			if (eSuccess == e)
			{
				const int cores{ CPU().getCPUCoreNumber() };
				ctx.set(ZMQ_IO_THREADS, &cores, sizeof(const int));
				dealer = ctx.create_socket(ZMQ_DEALER);

				if (dealer)
				{
					const int linger{ 0 };
					const std::string commID{ 
						boost::uuids::to_string(boost::uuids::random_generator()()) };

					dealer->setsockopt(ZMQ_LINGER, &linger, sizeof(const int));
					dealer->setsockopt(ZMQ_IDENTITY, commID.c_str(), commID.length());
					e = dealer->connect(address.c_str()) ? eBadConnect : eSuccess;

					if (eSuccess == e)
					{
						return ThreadPool::get_mutable_instance().createNewThread(
							boost::bind(&MajordomoWorker::pollerThreadProc, this)) ? eSuccess : eBadNewThread;
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
				e = eBadOperate;
			}

			return e;
		}

		int MajordomoWorker::stopWorker()
		{
			int e{ dealer && ctx.valid() ? eSuccess : eBadOperate };

			if (eSuccess == e)
			{
				stopped = true;
				dealer->close();
				ctx.destroy_socket(dealer);
				ctx.terminate();
				dealer = nullptr;
			}

			return e;
		}

		int MajordomoWorker::sendData(
			const std::string roleID, 
			const std::string flagID, 
			const std::string fromID, 
			const std::string toID, 
			const std::string data)
		{
			MessageData msg;
			msg.pushFront(data);
			msg.pushFront(toID);
			msg.pushFront(fromID);
			msg.pushFront(flagID);
			msg.pushFront(roleID);
			msg.pushFront("");
			return msg.sendData(dealer);
		}

		void MajordomoWorker::pollerThreadProc()
		{
			zmq_pollitem_t socketItems[] = {
					{ static_cast<void*>(dealer), 0, ZMQ_POLLIN, 0} };

			while (!stopped)
			{
				zmq_poll(socketItems, 1, 1);
				if (socketItems[0].revents & ZMQ_POLLIN)
				{
					MessageData msg;
					if (eSuccess == msg.recvData(dealer) && afterClientRecievedDataCallback)
					{
						//数据格式
						//-----------------------------------------------------------------------------------------
						//| CommID | Empty | worker / client | request / response / upload | FromID | ToID | Data |
						//-----------------------------------------------------------------------------------------

						//去掉空帧不处理
						msg.popFront();
						const std::string roleID{ msg.popFront() };
						const std::string flagID{ msg.popFront() };
						const std::string fromID{ msg.popFront() };
						const std::string toID{ msg.popFront() };
						const std::string data{ msg.popFront() };

						afterClientRecievedDataCallback(roleID, flagID, fromID, toID, data);
					}
				}
			}
		}
    }//namespace module
}//namespace mq
