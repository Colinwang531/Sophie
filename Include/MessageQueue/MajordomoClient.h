//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-08-18
//		Description :					Majordomo客户端类
//
//		History:						Author									Date										Description
//										王科威									2020-08-18									创建
//

#ifndef MQ_MODULE_MAJORDOMO_CLIENT_H
#define MQ_MODULE_MAJORDOMO_CLIENT_H

#include "ctx.hpp"
#include "socket_base.hpp"

namespace mq
{
    namespace module
    {
		class MajordomoClient
		{
		public:
			MajordomoClient(void* client = nullptr);
			virtual ~MajordomoClient(void);

		public:
			//启动Client实例
			//@address : "tcp://IP:PORT"
			//@Return : 错误码
			int startClient(const std::string address);

			//停止Client实例
			//@Return : 错误码
			int stopClient(void);

		private:
			//消息获取线程
			void pollerThreadProc(void);

		private:
			void* abstractClient;
			//0MQ context
			zmq::ctx_t ctx;
			//Socket for clients & workers
			zmq::socket_base_t* dealer;
			const std::string clientID;
		};//class MajordomoClient
    }//namespace module
}//namespace mq

#endif//MQ_MODULE_MAJORDOMO_CLIENT_H
