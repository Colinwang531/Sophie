//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-08-11
//		Description :					Majordomo服务端类
//
//		History:						Author									Date										Description
//										王科威									2020-08-11									创建
//

#ifndef MQ_MODULE_MAJORDOMO_WORKER_H
#define MQ_MODULE_MAJORDOMO_WORKER_H

#include "ctx.hpp"
#include "socket_base.hpp"

namespace mq
{
    namespace module
    {
		class MajordomoWorker 
		{
		public:
			MajordomoWorker(
				const std::string name, void* client = nullptr);
			virtual ~MajordomoWorker(void);

		public:
			//启动Worker实例
			//@address : "tcp://IP:PORT"
			//@Return : 错误码
			int startWorker(const std::string address);

			//停止Worker实例
			//@Return : 错误码
			int stopWorker(void);

			//发送消息
			//@flagID : Request/Response标识
			//@fromID : 发送者ID标识
			//@toID : 接收者ID标识
			//@msg : 消息数据
			//@Return : 错误码
			int sendData(
				const std::string flagID,
				const std::string fromID,
				const std::string toID,
				const std::string msg);

		private:
			//消息获取线程
			void pollerThreadProc(void);

			//Worker实例注册线程
			void autoRegisterToMajordomoBrokerThreadProc(void);

		private:
			const std::string workerName;
			void* abstractClient;
			//0MQ context
			zmq::ctx_t ctx;
			//Socket for clients & workers
			zmq::socket_base_t* dealer;
			const std::string workerID;
		};//class MajordomoWorker
    }//namespace module
}//namespace mq

#endif//MQ_MODULE_MAJORDOMO_WORKER_H
