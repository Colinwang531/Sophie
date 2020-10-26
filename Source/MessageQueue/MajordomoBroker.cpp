#include "boost/bind.hpp"
#include "Error.h"
#include "Hardware/Cpu.h"
using CPU = base::hardware::Cpu;
#include "Thread/ThreadPool.h"
using ThreadPool = base::thread::ThreadPool;
#include "MessageQueue/MessageData.h"
using MessageData = mq::message::MessageData;
#include "MessageQueue/MajordomoBroker.h"

namespace mq
{
    namespace module
    {
        MajordomoBroker::MajordomoBroker(
            AfterServerRecievedDataCallback callback /* = nullptr */)
            : afterServerRecievedDataCallback{ callback }, router{ nullptr }, stopped{ false }
        {}

        MajordomoBroker::~MajordomoBroker()
        {
            stopBroker();
        }

        int MajordomoBroker::startBroker(const std::string address)
        {
            int e{ ctx.valid() ? eSuccess : eBadOperate };
            
            if (eSuccess == e)
            {
				const int cores{ CPU().getCPUCoreNumber() };
				ctx.set(ZMQ_IO_THREADS, &cores, sizeof(const int));
				router = ctx.create_socket(ZMQ_ROUTER);

				if (router)
				{
					e = router->bind(address.c_str()) ? eBadBind : eSuccess;

					if (eSuccess == e)
					{
                        return ThreadPool::get_mutable_instance().createNewThread(
                            boost::bind(&MajordomoBroker::pollerThreadProc, this)) ? eSuccess : eBadNewThread;
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
                stopped = true;
                router->close();
                ctx.destroy_socket(router);
                ctx.terminate();
                router = nullptr;
            }

            return e;
        }

        int MajordomoBroker::sendData(
			const std::string commID,
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
            msg.pushFront(commID);
            return msg.sendData(router);
        }

        void MajordomoBroker::pollerThreadProc() 
        {
			zmq_pollitem_t socketItems[] = {
					{ static_cast<void*>(router), 0, ZMQ_POLLIN, 0} };

            while (!stopped) 
            {
                zmq_poll(socketItems, 1, 1);
                if (socketItems[0].revents & ZMQ_POLLIN)
                {
                    MessageData msg;
                    if (eSuccess == msg.recvData(router) && afterServerRecievedDataCallback)
                    {
                        //数据格式
						//-----------------------------------------------------------------------------------------
						//| CommID | Empty | worker / client | request / response / upload | FromID | ToID | Data |
					    //-----------------------------------------------------------------------------------------

                        const std::string commID{ msg.popFront() };
                        //去掉空帧不处理
                        msg.popFront();
                        const std::string roleID{ msg.popFront() };
                        const std::string flagID{ msg.popFront() };
                        const std::string fromID{ msg.popFront() };
                        const std::string toID{ msg.popFront() };
                        const std::string data{ msg.popFront() };

                        afterServerRecievedDataCallback(commID, roleID, flagID, fromID, toID, data);
                    }
                }
            }
        }
    }
}

