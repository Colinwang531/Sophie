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

#include "boost/function.hpp"
#include "ctx.hpp"

namespace mq
{
    namespace module
    {
		typedef boost::function<void(const std::string, const std::string, const std::string, const std::string, const std::string, const std::string)> AfterServerRecievedDataCallback;

		class MajordomoBroker
		{
		public:
			MajordomoBroker(
				AfterServerRecievedDataCallback callback = nullptr);
			virtual ~MajordomoBroker(void);

		public:
			//启动代理服务
			//@address : 服务端监听地址
			//@Return : 错误码
			//@Comment :  Bind broker to endpoint, can call this multiple times
			//			  We use a single socket for both clients and workers.
			int startBroker(const std::string address);

			//停止代理服务
			//@Return : 错误码
			int stopBroker(void);

			//发送数据
			//@commID : 通信ID标识
			//@roleID : 角色ID标识
			//@flagID : 标志ID标识
			//@fromID : 发送者ID标识
			//@toID : 接收者ID标识
			//@data : 消息数据
			//@Return : 错误码
			int sendData(
				const std::string commID,
				const std::string roleID,
				const std::string flagID,
				const std::string fromID,
				const std::string toID,
				const std::string data);

		private:
			//消息获取线程
			void pollerThreadProc(void);

		private:
			AfterServerRecievedDataCallback afterServerRecievedDataCallback;
			bool stopped;
			zmq::ctx_t ctx;
			zmq::socket_base_t* router;
		};//class MajordomoBroker
    }//namespace module
}//namespace mq

#endif//MQ_MODULE_MAJORDOMO_BROKER_H
