#include "boost/format.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "zmq.h"
#include "socket_base.hpp"
#include "Error.h"
#include "Define.h"
#include "Network/ZMQ/Msg_.h"
#include "Network/ZMQ/Core/Dealer_.h"

namespace framework
{
    namespace network
    {
		namespace zeromq
		{
			Dealer::Dealer()
			{}
			Dealer::~Dealer()
			{}

			int Dealer::bind(
				void* s /* = nullptr */,
				const unsigned short port /* = 0 */)
			{
				int e{
					s && gMinPortNumber < port&& gMaxPortNumber > port ? eSuccess : eInvalidParameter };

				if (eSuccess == e)
				{
					zmq::socket_base_t* so{ reinterpret_cast<zmq::socket_base_t*>(s) };
					e = so->bind((boost::format("tcp://*:%d") % port).str().c_str()) ? eBadBind : eSuccess;
				}

				return e;
			}

			int Dealer::connect(
				void* s /* = nullptr */, 
				const char* ipv4 /* = nullptr */, 
				const unsigned short port /* = 0 */)
			{
				int e{ 
					s && ipv4 && gMaxPortNumber > port && gMinPortNumber < port ? eSuccess : eInvalidParameter };

				if (eSuccess == e)
				{
					zmq::socket_base_t* so{ reinterpret_cast<zmq::socket_base_t*>(s) };

					//开启TCP保活机制防止网络连接因长时间无数据而被中断
					int keepalive{ 1 };
					so->setsockopt(ZMQ_TCP_KEEPALIVE, &keepalive, sizeof(int));

					//网络连接空闲30s即发送保活包
					int idle{ 30 };
					so->setsockopt(ZMQ_TCP_KEEPALIVE_IDLE, &idle, sizeof(int));

					//设置ID标识
					const std::string commID{
							boost::uuids::to_string(boost::uuids::random_generator()()) };
					so->setsockopt(ZMQ_IDENTITY, commID.c_str(), commID.length());

					e = so->connect((boost::format("tcp://%s:%d") % ipv4 % port).str().c_str()) ? eBadConnect : eSuccess;
				}

				return e;
			}

			int Dealer::send(
				void* s /* = nullptr */,
				Msg* msg /* = nullptr */)
			{
				return s && msg ? msg->sendData(s) : eInvalidParameter;
			}
		}//namespace zeromq
    }//namespace network
}//namespace framework
