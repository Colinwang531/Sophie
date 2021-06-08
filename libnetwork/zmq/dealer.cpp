#include "boost/format.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "zmq.h"
#include "const.h"
#include "error.h"
#include "dealer.h"

namespace framework
{
    namespace libnetwork
    {
		namespace zmq
		{
			Dealer::Dealer()
			{}
			Dealer::~Dealer()
			{}

			int Dealer::connect(
				const std::string ipv4, 
				const unsigned short port /* = 0 */,
				void* s /* = nullptr */)
			{
				CommonError e{ 
					s && !ipv4.empty() && gMinPortNumber <= port && gMaxPortNumber >= port ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER };

				if (CommonError::COMMON_ERROR_SUCCESS == e)
				{
					//开启TCP保活机制防止网络连接因长时间无数据而被中断
					int keepalive{ 1 };
					zmq_setsockopt(s, ZMQ_TCP_KEEPALIVE, &keepalive, sizeof(int));
					//网络连接空闲30s即发送保活包
					int idle{ 30 };
					zmq_setsockopt(s, ZMQ_TCP_KEEPALIVE_IDLE, &idle, sizeof(int));
					//设置ID标识
					const std::string commID{
							boost::uuids::to_string(boost::uuids::random_generator()()) };
					zmq_setsockopt(s, ZMQ_IDENTITY, commID.c_str(), commID.length());

					zmq_connect(s, (boost::format("tcp://%s:%d") % ipv4 % port).str().c_str());
				}

				return (int)e;
			}

			int Dealer::send(
				const std::string data,
				void* s /* = nullptr */)
			{
				CommonError e{
					!data.empty() && s ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER};

				if (CommonError::COMMON_ERROR_SUCCESS == e)
				{

				}

				return (int)e;
			}
		}//namespace zmq
    }//namespace libnetwork
}//namespace framework
