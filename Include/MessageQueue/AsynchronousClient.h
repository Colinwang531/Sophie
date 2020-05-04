//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-03
//		Description :					异步客户端类
//
//		History:						Author									Date										Description
//										王科威									2020-05-03									创建
//

#ifndef ASYNCHRONOUS_CLIENT_H
#define ASYNCHRONOUS_CLIENT_H

#include <vector>

namespace mq
{
	namespace module
	{
		class AsynchronousClient
		{
		public:
			AsynchronousClient(void);
			virtual ~AsynchronousClient(void);

		public:
			//启动客户端
			//@address : 服务端地址
			//@port : 端口号
			//@Return : 错误码值
			int startClient(const char* address = nullptr, const unsigned short port = 61001);

			//停止客户端
			//@Return : 错误码值
			int stopClient(void);

//			int send(const char* data, const int dataBytes, std::string& resp);

		protected:
			//创建客户端模型
			//继承类在该方法中实现自定义模型的创建
			//@address : 服务端地址
			//@Return : 错误码值
			virtual int createNewModule(const char* address = nullptr);

			//销毁客户端模型
			//@Return : 错误码值
			virtual int destroyModule(void);

			//添加数据读取实例项
			//@items : 数据读取实例数组
			virtual void addPollItem(std::vector<void*>& items);

			//数据读取处理
			//@s : 数据读取的socket实例
			virtual void afterPollItemMessage(void* s = nullptr);

		private:
			//启动异步客户端数据读取
			//@Return : 错误码值
			int startPoller(void);
			static void pollerThreadProc(void* param = nullptr);
			
		protected:
			void* ctx;

		private:
			void* dealer;
			bool stopped;
			void* workerThread;
		};//class AsynchronousClient
	}//namespace module
}//namespace mq

#endif//ASYNCHRONOUS_CLIENT_H
