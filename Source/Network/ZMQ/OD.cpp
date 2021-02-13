#include "zmq.h"
#include "socket_base.hpp"
#include "Error.h"
#include "Thread/ThreadPool.h"
using ThreadPool = framework::thread::ThreadPool;
#include "Network/ZMQ/Core/Ctx_.h"
#include "Network/ZMQ/Core/Dealer_.h"
#include "Network/ZMQ/Msg_.h"
#include "Network/ZMQ/OD.h"

namespace framework
{
	namespace network
	{
		namespace zeromq
		{
			OD::OD() : stopped{ false }
			{}
			OD::~OD()
			{}

			int OD::startOD(
				const char* ipv4 /* = nullptr */, 
				const unsigned short port /* = 0 */)
			{
				int e{ Ctx().get_mutable_instance().init() };

				if (eSuccess == e)
				{
					so = Ctx().get_mutable_instance().createNewSocket(ZMQ_DEALER);

					if (so)
					{
						e = Dealer().connect(so, ipv4, port);
						e = ThreadPool::get_mutable_instance().createNewThread(
							boost::bind(&OD::pollMessageWorkerThread, this)) ? eSuccess : eBadNewThread;
					}
					else
					{
						stopOD();
					}
				}

				return e;
			}

			int OD::stopOD()
			{
				stopped = true;
				Ctx().get_mutable_instance().destroySocket(so);
				return Ctx().get_mutable_instance().uninit();
			}

			int OD::sendMsg(Msg* msg /* = nullptr */)
			{
				return msg ? Dealer().send(so, msg) : eInvalidParameter;
			}

			void OD::pollMessageWorkerThread()
			{
				zmq::socket_base_t* ds{ reinterpret_cast<zmq::socket_base_t*>(so) };
				zmq_pollitem_t pollitems[] = { { ds, 0, ZMQ_POLLIN, 0} };

				while (!stopped)
				{
					Msg msg;
					zmq_poll(pollitems, 1, 1);
					if (pollitems[0].revents & ZMQ_POLLIN)
					{
						if (eSuccess == msg.recvData(so))
						{
							parsePolledMessage(msg);
						}
					}
				}
			}

			void OD::parsePolledMessage(Msg& msg)
			{
				//---------------------------------------------------------------------------------------------------------------------------------------
				//| "" | module | from | to | router(1) | ... | router(n) | "" | extend(1) | ... | extend(n) | sequence | timestamp | command | message |
				//---------------------------------------------------------------------------------------------------------------------------------------

				//解析固定大小段数据
				msg.removeMessage();
				const std::string module{ msg.removeMessage() };
				const std::string from{ msg.removeMessage() };
				const std::string to{ msg.removeMessage() };

				//解析路由表数据
				std::vector<std::string> routers;
				while (1)
				{
					std::string item{ msg.removeMessage() };

					if (item.empty())
					{
						break;
					}
					else
					{
						routers.push_back(item);
					}
				}

				//解析消息数据
				std::vector<std::string> messages;
				while (1)
				{
					std::string item{ msg.removeMessage() };

					if (item.empty())
					{
						break;
					}
					else
					{
						messages.push_back(item);
					}
				}

				afterParsePolledMessage(module, from, to, routers, messages);
			}
		}//namespace zeromq
	}//namespace network
}//namespace framework
