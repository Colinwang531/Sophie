//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-04-26
//		Description :					服务端抽象类
//
//		History:						Author									Date										Description
//										王科威									2020-04-26									创建
//										王科威									2020-08-17									1.定义为抽象基类
//										     									          									2.新增服务端类型标识
//										     									          									3.分离服务端实例和实例接口
//

#ifndef ABSTRACT_SERVER_H
#define ABSTRACT_SERVER_H

#include <vector>

namespace base
{
	namespace network
	{
		typedef enum class tagNetworkServerType_t : int
		{
			NETWORK_SERVER_TYPE_NONE = 0,
			NETWORK_SERVER_TYPE_ASIO,
			NETWORK_SERVER_TYPE_MAJORDOMO_BROKER,
			NETWORK_SERVER_TYPE_ASYNC_DISPATHER
		}NetworkServerType;

		class AbstractServer
		{
		public:
			AbstractServer(
				const NetworkServerType type = NetworkServerType::NETWORK_SERVER_TYPE_NONE);
			virtual ~AbstractServer(void);

		public:
			//启动服务端
			//@listenPort : 本地监听端口号
			//@upstreamIP : 上游服务端IP地址
			//@upstreamPort : 上游服务端端口号
			//@Return : 错误码
			//@Comment : 如果不需要连接上游服务端时,upstreamIP和upstreamPort这2个参数是可以不设置的
			int startServer(
				const unsigned short listenPort = 61001, 
				const char* upstreamIP = nullptr, 
				const unsigned short upstreamPort = 61101);

			//停止服务端
			//@Return : 错误码
 			int stopServer(void);

		protected:
			//创建本地服务端通信模型
			//@listenPort : 本地监听端口号
			//@Return : 错误码
			//@Comment : listenPort参数必须在该类中定义,用于本地监听端口
			//			 第二个和第三个参数由派生类定义,用于上游连接地址和端口
			virtual int createNewModule(
				const unsigned short listenPort = 61001, 
				const char* = nullptr, 
				const unsigned short = 0);

			//销毁服务端模型
			//@Return : 错误码
			virtual int destroyModule(void);

			//启动数据读取线程
			//@Return : 错误码
			virtual int startPoller(void);

			//数据处理接口
			//@s : SOCKET标识
			//@id : ID标识字符串
			//@idbytes : ID标识字节数
			//@delimiter : 分隔符字符串
			//@delimiterbytes : 分隔符字节数
			//@data : 数据内容字符串
			//@databytes : 数据内容字节数
			//@Comment : 数据处理的实现必须由派生类完成,基类不处理任何数据
			virtual void afterServerPollMessage(
				void* s = nullptr,
				const void* id = nullptr, 
				const unsigned int idbytes = 0,
				const void* delimiter = nullptr, 
				const unsigned int delimiterbytes = 0,
				const void* data = nullptr, 
				const unsigned int databytes = 0) = 0;

		private:
			//消息转发线程
			void pollerThreadProc(void);

			//启动异步服务端处理线程
			//@Return : 错误码
			int startWorker(void);

			//消息处理线程
			void workerThreadProc(void);

			//消息分发传递
			//@src : 数据源ID
			//@dest : 目的ID
			void transferData(void* src = nullptr, void* dest = nullptr);

		protected:
			void* ctx;
			//线程退出标识
			bool stopped;

		private:
			void* router;
			void* dealer;
		};//class AsynchronousServer
	}//namespace module
}//namespace mq

#endif//ABSTRACT_SERVER_H
