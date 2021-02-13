#include "boost/format.hpp"
#include "zmq.h"
#include "socket_base.hpp"
#include "Error.h"
#include "Thread/ThreadPool.h"
using ThreadPool = framework::thread::ThreadPool;
#include "Network/ZMQ/Core/Ctx_.h"
#include "Network/ZMQ/Core/Router_.h"
#include "Network/ZMQ/Core/Dealer_.h"
#include "Network/ZMQ/Msg_.h"
#include "Network/ZMQ/RD.h"

namespace framework
{
	namespace network
	{
		namespace zeromq
		{
			RD::RD() : stopped{ false }
			{
				for (int i = 0; i != SO_NUMBER; ++i)
				{
					sockets[i] = nullptr;
				}
			}
			RD::~RD()
			{}

			int RD::startRD(
				const unsigned short rlp /* = 0 */, 
				const char* dcip /* = nullptr */, 
				const unsigned short dcp /* = 0 */)
			{
				int e{ Ctx().get_mutable_instance().init() };

				if (eSuccess == e)
				{
					sockets[0] = Ctx().get_mutable_instance().createNewSocket(ZMQ_ROUTER);
					sockets[1] = Ctx().get_mutable_instance().createNewSocket(ZMQ_DEALER);

					//必须2个同时有效才可以
					if (sockets[0] && sockets[1])
					{
						e = Router().bind(sockets[0], rlp);
						e = Dealer().connect(sockets[1], dcip, dcp);
						e = ThreadPool::get_mutable_instance().createNewThread(
							boost::bind(&RD::pollMessageWorkerThread, this)) ? eSuccess : eBadNewThread;
					}
					else
					{
						stopRD();
					}
				}

				return e;
			}

			int RD::startRD(const unsigned short rlp /* = 0 */, const unsigned short dlp /* = 0 */)
			{
				int e{ Ctx().get_mutable_instance().init() };

				if (eSuccess == e)
				{
					sockets[0] = Ctx().get_mutable_instance().createNewSocket(ZMQ_ROUTER);
					sockets[1] = Ctx().get_mutable_instance().createNewSocket(ZMQ_DEALER);

					//必须2个同时有效才可以
					if (sockets[0] && sockets[1])
					{
						e = Router().bind(sockets[0], rlp);
						e = Dealer().bind(sockets[1], dlp);
						e = ThreadPool::get_mutable_instance().createNewThread(
							boost::bind(&RD::pollMessageWorkerThread, this)) ? eSuccess : eBadNewThread;
					}
					else
					{
						stopRD();
					}
				}

				return e;
			}

			int RD::stopRD()
			{
				stopped = true;
				for (int i = 0; i != SO_NUMBER; ++i)
				{
					Ctx().get_mutable_instance().destroySocket(sockets[i]);
				}

				return Ctx().get_mutable_instance().uninit();
			}

			int RD::sendMsg(
				const int idx /* = -1 */,
				Msg* msg /* = nullptr */)
			{
				int e{ -1 < idx && SO_NUMBER > idx && msg ? eSuccess : eInvalidParameter };

				if (eSuccess == e)
				{
					e = (0 == idx ? Router().send(sockets[idx], msg) : Dealer().send(sockets[idx], msg));
				}

				return e;
			}

			void RD::pollMessageWorkerThread()
			{
				zmq::socket_base_t* rs{ reinterpret_cast<zmq::socket_base_t*>(sockets[0]) };
				zmq::socket_base_t* ds{ reinterpret_cast<zmq::socket_base_t*>(sockets[1]) };
				zmq_pollitem_t pollitems[] = { { rs, 0, ZMQ_POLLIN, 0}, { ds, 0, ZMQ_POLLIN, 0} };

				while (!stopped)
				{
					Msg msg;
					zmq_poll(pollitems, SO_NUMBER, 1);
					if (pollitems[0].revents & ZMQ_POLLIN)
					{
						if (eSuccess == msg.recvData(sockets[0]))
						{
							parsePolledMessage(msg);
						}
					}
					else if (pollitems[1].revents & ZMQ_POLLIN)
					{
						if (eSuccess == msg.recvData(sockets[1]))
						{
							parsePolledMessage(msg);
						}
					}
				}
			}

			void RD::parsePolledMessage(Msg& msg)
			{
				//------------------------------------------------------------------------------------------------------------------------------------------------
				//| sender | "" | module | from | to | router(1) | ... | router(n) | "" | extend(1) | ... | extend(n) | sequence | timestamp | command | message |
				//------------------------------------------------------------------------------------------------------------------------------------------------

				//解析固定大小段数据
				const std::string sender{ msg.removeMessage() };
				if (!sender.empty())
				{
					msg.removeMessage();
				}
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

				if (sender.empty())
				{
					afterParsePolledMessage(module, from, to, routers, messages);
				} 
				else
				{
					afterParsePolledMessage(sender, module, from, to, routers, messages);
				}
			}
		}//namespace zeromq
	}//namespace network
}//namespace framework
