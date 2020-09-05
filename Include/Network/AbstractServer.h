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

#ifndef BASE_NETWORK_ABSTRACT_SERVER_H
#define BASE_NETWORK_ABSTRACT_SERVER_H

#include <vector>

namespace base
{
	namespace network
	{
		typedef enum class tagServerModuleType_t : int
		{
			SERVER_MODULE_TYPE_NONE = 0,
			SERVER_MODULE_TYPE_ASIO,
			SERVER_MODULE_TYPE_MAJORDOMO_BROKER,
			SERVER_MODULE_TYPE_ASYNC_DISPATHER
		}ServerModuleType;

		class AbstractServer
		{
		public:
			AbstractServer(
				const ServerModuleType type = ServerModuleType::SERVER_MODULE_TYPE_NONE);
			virtual ~AbstractServer(void);

		public:
			//启动服务端
			//@listenPort : 本地监听端口号
			//@Return : 错误码
			int startServer(const unsigned short listenPort = 61001);

			//停止服务端
			//@Return : 错误码
 			int stopServer(void);

			//获取服务端运行/停止状态
			//@Return : 服务端运行/停止状态
			//@Comment : 服务端运行/停止状态只能由服务端内部修改,外部只能获取该值
			inline const bool isStopped(void) const
			{
				return stopped;
			}

			//发送消息
			//@commID : 通信ID标识
			//@flagID : Request/Response标识
			//@fromID : 发送者ID标识
			//@toID : 接收者ID标识
			//@msg : 消息数据
			//@Return : 错误码
			int sendMessageData(
				const std::string commID,
				const std::string flagID,
				const std::string fromID,
				const std::string toID,
				const std::string msg);

			//连接超时处理
			virtual void afterAutoCheckConnectionTimeoutProcess(void) = 0;

			//服务端数据接收处理
			//@commID : 通信ID标识
			//@flagID : Request/Response标识
			//@fromID : 发送者ID标识
			//@toID : 接收者ID标识
			//@msg : 消息数据
			virtual void afterServerPolledMessageProcess(
				const std::string commID, 
				const std::string flagID,
				const std::string fromID,
				const std::string toID,
				const std::string msg) = 0;

		protected:
			//创建本地服务端模型
			//@address : 本地监听地址
			//@Return : 错误码
			virtual int createNewServerModule(const std::string address);

			//销毁本地服务端模型
			//@Return : 错误码
			virtual int destroyServerModule(void);

		protected:
			const ServerModuleType serverModuleType;
			void* serverModule;
			bool stopped;
		};//class AbstractServer
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_ABSTRACT_SERVER_H
