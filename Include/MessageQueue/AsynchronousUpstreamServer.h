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
			//创建上游服务端连接模型
			//@port : 上游服务端端口号
			//@upstream : 上游服务端连接地址
			//@Return : 错误码值
			int createNewModule(const unsigned short port = 61001, const char* address = nullptr) override;

			//销毁服务端模型
			//@Return : 错误码值
			int destroyModule(void) override;

			//添加数据读取实例项
			//@items : 数据读取实例数组
			void addPollItem(std::vector<void*>& items) override;

			//数据读取处理
			//@s : 数据读取的socket实例
			void afterPollItemMessage(void* s = nullptr) override;

		private:
			void* dealer;
		};//class AsynchronousUpstreamServer
	}//namespace module
}//namespace mq

#endif//ASYNCHRONOUS_UPSTREAM_SERVER_H
