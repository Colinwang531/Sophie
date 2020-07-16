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
			//@serverIP : 服务端IP地址
			//@serverPort : 服务端端口号
			//@Return : 错误码
			int startClient(
				const char* serverIP = nullptr,
				const unsigned short serverPort = 61001);

			//停止客户端
			//@Return : 错误码
			int stopClient(void);

		protected:
			//创建客户端通信模型
			//@serverIP : 服务端IP地址
			//@serverPort : 服务端端口号
			//@Return : 错误码
			virtual int createNewModule(
				const char* serverIP = nullptr, 
				const unsigned short serverPort = 61001);

			//销毁客户端模型
			//@Return : 错误码
			virtual int destroyModule(void);

			//启动数据读取线程
			//@Return : 错误码
			virtual int startPoller(void);

			//数据处理接口
			//@data : 数据内容字符串
			//@databytes : 数据内容字节数
			//@Comment : 数据处理的实现必须由派生类完成,基类不处理任何数据
			virtual void afterClientPollMessage(
				const void* data = nullptr, 
				const unsigned int databytes = 0) = 0;

		private:
			//消息处理线程
			void pollerThreadProc(void);
			
		protected:
			void* ctx;
			void* dealer;
			//线程退出标识
			bool stopped;
		};//class AsynchronousClient
	}//namespace module
}//namespace mq

#endif//ASYNCHRONOUS_CLIENT_H
