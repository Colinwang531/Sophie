#include "boost/bind.hpp"
#include "boost/checked_delete.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/make_shared.hpp"
#include "boost/thread.hpp"
#include "zmq.h"
#include "Error.h"
#include "Hardware/Cpu.h"
using CPU = base::hardware::Cpu;
#include "Time/XTime.h"
using Time = base::time::Time;
#include "Thread/ThreadPool.h"
using ThreadPool = base::thread::ThreadPool;
#include "Network/AbstractServer.h"
using AbstractServer = base::network::AbstractServer;
#include "MessageQueue/MajordomoBroker.h"

namespace mq
{
    namespace module
    {
        MajordomoBroker::MajordomoBroker(void* server /* = nullptr */)
            : abstractServer{ server }
        {}

        MajordomoBroker::~MajordomoBroker()
        {
            stopBroker();
        }

        int MajordomoBroker::startBroker(const std::string address)
        {
            int e{ address.empty() ? eInvalidParameter : eSuccess };
            
            if (eSuccess == e && ctx.valid())
            {
				const int cores{ CPU().getCPUCoreNumber() };
				ctx.set(ZMQ_IO_THREADS, &cores, sizeof(const int));
				router = ctx.create_socket(ZMQ_ROUTER);

				if (router)
				{
					e = router->bind(address.c_str()) ? eBadBind : eSuccess;

					if (eSuccess == e)
					{
						void* t1{
							ThreadPool::get_mutable_instance().createNewThread(
								boost::bind(&MajordomoBroker::pollerThreadProc, this)) };
						void* t2{
							ThreadPool::get_mutable_instance().createNewThread(
								boost::bind(&MajordomoBroker::autoCheckWorkerTimeoutThreadProc, this)) };
						return t1 && t2 ? eSuccess : eBadNewThread;
					}
					else
					{
						stopBroker();
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

        int MajordomoBroker::stopBroker()
        {
            int e{ router && ctx.valid() ? eSuccess : eBadOperate };

            if (eSuccess == e)
            {
                router->close();
                ctx.destroy_socket(router);
                ctx.terminate();
                router = nullptr;
            }

            return e;
        }

        int MajordomoBroker::sendData(
            const std::string commID, 
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
            md.pushFront(commID);
            return md.sendData(router);
        }

        void MajordomoBroker::pollerThreadProc() 
        {
            AbstractServer* as{ reinterpret_cast<AbstractServer*>(abstractServer) };
			zmq_pollitem_t socketItems[] = {
					{ static_cast<void*>(router), 0, ZMQ_POLLIN, 0} };

            while (as && !as->isStopped()) 
            {
                zmq_poll(socketItems, 1, 1);
                if (socketItems[0].revents & ZMQ_POLLIN)
                {
                    MessageData msgdata;
                    if (eSuccess == msgdata.recvData(router))
                    {
                        //CommID
                        const std::string commID{ msgdata.popFront() };
                        //Delimiter
                        msgdata.popFront();
                        //Request/Response
                        const std::string flagID{ msgdata.popFront() };
                        //FromID
                        const std::string fromID{ msgdata.popFront() };
                        //ToID
                        const std::string toID{ msgdata.popFront() };
                        //Data
                        const std::string data{ msgdata.popFront() };

                        as->afterServerPolledMessageProcess(commID, flagID, fromID, toID, data);
                    }
                }
            }
        }

        void MajordomoBroker::autoCheckWorkerTimeoutThreadProc()
        {
            AbstractServer* as{ reinterpret_cast<AbstractServer*>(abstractServer) };
            //超时时间90秒
            long long startTime{ Time().tickcount() };

            while (as && !as->isStopped())
            {
                long long nowTime{ Time().tickcount() };

				//每30秒检测一次
				if (30000 < nowTime - startTime)
				{
                    as->afterAutoCheckConnectionTimeoutProcess();
					startTime = nowTime;
				}

                boost::this_thread::sleep(boost::posix_time::seconds(1));
            }
        }
    }
}

