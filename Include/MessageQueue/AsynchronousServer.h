//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-04-26
//		Description :					异步服务端类
//
//		History:						Author									Date										Description
//										王科威									2020-04-26									创建
//

#ifndef ASYNCHRONOUS_SERVER_H
#define ASYNCHRONOUS_SERVER_H

#include <vector>

namespace mq
{
	namespace module
	{
		class AsynchronousServer
		{
		public:
			AsynchronousServer(void);
			virtual ~AsynchronousServer(void);

		public:
			//启动服务端
			//@port : 服务端监听端口号
			//@upstreamAddress : 上游服务端地址
			//@upstreamPort : 上游服务端端口号
			//@Return : 错误码值
			int startServer(
				const unsigned short port = 61001, const char* upstreamAddress = nullptr, const unsigned short upstreamPort = 61101);

			//停止服务端
			//@Return : 错误码值
 			int stopServer(void);

		protected:
			//创建服务端模型
			//继承类在该方法中实现自定义模型的创建
			//@port : 服务端监听端口号
			//@Return : 错误码值
			virtual int createNewModule(const unsigned short port = 61001, const char* address = nullptr);

			//销毁服务端模型
			//@Return : 错误码值
			virtual int destroyModule(void);

			//添加数据读取实例项
			//@items : 数据读取实例数组
			virtual void addPollItem(std::vector<void*>& items);

			//数据读取处理
			//@s : 数据读取的socket实例
			virtual void afterPollItemMessage(void* s = nullptr);

		private:
			//启动异步服务端数据读取
			//@Return : 错误码值
			int startPoller(void);
			static void pollerThreadProc(void* param = nullptr);

			//启动异步服务端处理线程
			//@Return : 错误码值
			int startWorker(void);
			static void workerThreadProc(void* param = nullptr);

			//消息分发传递
			//@src : 数据源ID
			//@dest : 目的ID
			void transferData(void* src = nullptr, void* dest = nullptr);

		protected:
			void* ctx;

		private:
			void* router;
			void* dealer;
			//线程退出标识
			bool stopped;
			//服务端线程组
			//存储Poller和Worker线程实例
			std::vector<void*> threadGroup;
		};//class AsynchronousServer
	}//namespace module
}//namespace mq

#endif//ASYNCHRONOUS_SERVER_H
