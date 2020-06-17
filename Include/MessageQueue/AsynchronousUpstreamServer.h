//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-04-30
//		Description :					异步上游服务端类
//
//		History:						Author									Date										Description
//										王科威									2020-04-30									创建
//

#ifndef ASYNCHRONOUS_UPSTREAM_SERVER_H
#define ASYNCHRONOUS_UPSTREAM_SERVER_H

#include "AsynchronousServer.h"

namespace mq
{
	namespace module
	{
		class AsynchronousUpstreamServer : public AsynchronousServer
		{
		public:
			AsynchronousUpstreamServer(void);
			virtual ~AsynchronousUpstreamServer(void);

		protected:
			//创建上游服务端通信模型
			//@listenPort : 本地监听端口号
			//@upstreamIP : 上游服务端IP地址
			//@upstreamPort : 上游服务端端口号
			//@Return : 错误码
			int createNewModule(
				const unsigned short listenPort = 61001, 
				const char* upstreamIP = nullptr, 
				const unsigned short upstreamPort = 61101) override;

			//销毁上游服务端模型
			//@Return : 错误码值
			int destroyModule(void) override;

			//启动数据读取线程
			//@Return : 错误码
			int startPoller(void) override;

			//数据处理接口
			//@msg : 数据内容
			//@bytes : 数据字节数
			virtual void afterUpstreamPollMessage(
				const char* msg = nullptr, 
				const unsigned int bytes = 0) = 0;

		private:
			//上游消息处理线程
			void pollerThreadProc(void);

		private:
			void* dealer;
		};//class AsynchronousUpstreamServer
	}//namespace module
}//namespace mq

#endif//ASYNCHRONOUS_UPSTREAM_SERVER_H
