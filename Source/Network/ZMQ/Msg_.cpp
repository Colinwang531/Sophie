#include "msg.hpp"
#include "socket_base.hpp"
#include "Define.h"
#include "Error.h"
#include "Network/ZMQ/Msg_.h"

namespace framework
{
    namespace network
    {
		namespace zeromq
		{
			Msg::Msg() {}
			Msg::~Msg() {}

			void Msg::addMessage(const std::string msg)
			{
				msgDataGroup.insert(msgDataGroup.begin(), msg);
			}

			const std::string Msg::removeMessage()
			{
				std::string msg;

				if (0 < msgDataGroup.size())
				{
					msg = msgDataGroup.front();
					msgDataGroup.erase(msgDataGroup.begin());
				}

				return msg;
			}

			int Msg::recvData(void* s /* = nullptr */)
			{
				int e{ eInvalidParameter };

				while (s)
				{
					zmq::msg_t zmsg;
					zmsg.init();
					zmq::socket_base_t* zsb{ reinterpret_cast<zmq::socket_base_t*>(s) };

					e = zsb->recv(&zmsg, ZMQ_DONTWAIT);
					if (eSuccess == e)
					{
						const std::string msg{ static_cast<const char*>(zmsg.data()), zmsg.size() };
						msgDataGroup.push_back(msg);

						int more{ 0 };
						size_t bytes{ sizeof(more) };
						zsb->getsockopt(ZMQ_RCVMORE, &more, &bytes);
						//没有可接收的数据就可以结束了
						if (!more)
						{
							break;
						}
					}
					else
					{
						break;
					}
				}

				return e;
			}

			int Msg::sendData(void* s /* = nullptr */)
			{
				int e{ s ? eSuccess : eInvalidParameter };

				if (eSuccess == e)
				{
					const int msgNo{ (const int)msgDataGroup.size() };

					for (int i = 0; i != msgNo; ++i)
					{
						const std::string msg{ msgDataGroup[i] };
						const size_t msglen{ msg.length() };
						zmq::msg_t zmsg;
						e = zmsg.init_size(msglen);

						if (eSuccess == e)
						{
#ifdef WINDOWS
							memcpy_s(zmsg.data(), msglen, msg.c_str(), msglen);
#else
							memcpy(zmsg.data(), msg.c_str(), msglen);
#endif//WINDOWS
							zmq::socket_base_t* zsb{ reinterpret_cast<zmq::socket_base_t*>(s) };
							e = zsb->send(&zmsg, i < msgNo - 1 ? ZMQ_DONTWAIT | ZMQ_SNDMORE : ZMQ_DONTWAIT) ? eBadSend : eSuccess;
						}
						else
						{
							break;
						}
					}
				}

				return e;
			}
		}//namespace zeromq
    }//namespace network
}//namespace framework
