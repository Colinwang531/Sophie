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

#include "boost/function.hpp"
#include "ctx.hpp"

namespace mq
{
    namespace module
    {
		typedef boost::function<void(const std::string, const std::string, const std::string, const std::string, const std::string)> AfterClientRecievedDataCallback;

		class MajordomoWorker 
		{
		public:
			MajordomoWorker(
				AfterClientRecievedDataCallback callback = nullptr);
			virtual ~MajordomoWorker(void);

		public:
			//启动Worker实例
			//@address : 服务端监听地址
			//@Return : 错误码
			int startWorker(const std::string address);

			//停止Worker实例
			//@Return : 错误码
			int stopWorker(void);

			//发送消息
			//@roleID : 角色ID标识
			//@flagID : 标志ID标识
			//@fromID : 发送者ID标识
			//@toID : 接收者ID标识
			//@data : 消息数据
			//@Return : 错误码
			int sendData(
				const std::string roleID,
				const std::string flagID,
				const std::string fromID,
				const std::string toID,
				const std::string data);

		private:
			//消息获取线程
			void pollerThreadProc(void);

		private:
			AfterClientRecievedDataCallback afterClientRecievedDataCallback;
			bool stopped;
			zmq::ctx_t ctx;
			zmq::socket_base_t* dealer;
		};//class MajordomoWorker
    }//namespace module
}//namespace mq

#endif//MQ_MODULE_MAJORDOMO_WORKER_H
