//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-04-26
//		Description :					Majordomo服务端类
//
//		History:						Author									Date										Description
//										王科威									2020-04-26									创建
//

#ifndef MQ_MODULE_MAJORDOMO_BROKER_H
#define MQ_MODULE_MAJORDOMO_BROKER_H

// #include "DataStruct/UnorderedMap.h"
// #include "MajordomoInfo.h"
#include "MessageData.h"
using MessageData = mq::message::MessageData;

namespace mq
{
    namespace module
    {
		class MajordomoBroker
		{
		public:
			MajordomoBroker(void* server = nullptr);
			virtual ~MajordomoBroker(void);

		public:
			//启动代理服务
			//@address : "tcp://IP:PORT"
			//@Return : 错误码
			//@Comment :  Bind broker to endpoint, can call this multiple times
			//			  We use a single socket for both clients and workers.
			int startBroker(const std::string address);

			//停止代理服务
			//@Return : 错误码
			int stopBroker(void);

			//发送数据
			//@commID : 通信ID标识
			//@flagID : Request/Response标识
			//@fromID : 发送者ID标识
			//@toID : 接收者ID标识
			//@msg : 消息数据
			//@Return : 错误码
			int sendData(
				const std::string commID, 
				const std::string flagID,
				const std::string fromID,
				const std::string toID,
				const std::string msg);

		private:
			//消息获取线程
			void pollerThreadProc(void);

			//Worker实例超时检测线程
			void autoCheckWorkerTimeoutThreadProc(void);

		private:
			void* abstractServer;
			//0MQ context
			zmq::ctx_t ctx;
			//Socket for clients & workers
			zmq::socket_base_t* router;
		};//class MajordomoBroker
    }//namespace module
}//namespace mq

#endif//MQ_MODULE_MAJORDOMO_BROKER_H
