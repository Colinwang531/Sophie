//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-03
//		Description :					客户端抽象类
//
//		History:						Author									Date										Description
//										王科威									2020-05-03									创建
//										王科威									2020-08-17									1.定义为抽象基类
//										     									          									2.新增服务端类型标识
//										     									          									3.分离服务端实例和实例接口
//

#ifndef BASE_NETWORK_ABSTRACT_CLIENT_H
#define BASE_NETWORK_ABSTRACT_CLIENT_H

namespace base
{
	namespace network
	{
		typedef enum class tagClientModuleType_t : int
		{
			CLIENT_MODULE_TYPE_NONE = 0,
			CLIENT_MODULE_TYPE_MAJORDOMO_WORKER,
			CLIENT_MODULE_TYPE_MAJORDOMO_CLIENT,
		}ClientModuleType;

		class AbstractClient
		{
		public:
			AbstractClient(
				const ClientModuleType type = ClientModuleType::CLIENT_MODULE_TYPE_NONE);
			virtual ~AbstractClient(void);

		public:
			//启动客户端
			//@address : 服务端地址
			//@name : 服务端名称
			//@Return : 错误码
			int startClient(
				const std::string address,
				const std::string name);

			//停止客户端
			//@Return : 错误码
			int stopClient(void);

			//获取客户端运行/停止状态
			//@Return : 客户端运行/停止状态
			//@Comment : 客户端运行/停止状态只能由服务端内部修改,外部只能获取该值
			inline const bool isStopped(void) const
			{
				return stopped;
			}

			//发送消息
			//@flagID : Request/Response标识
			//@fromID : 发送者ID标识
			//@toID : 接收者ID标识
			//@msg : 消息数据
			//@Return : 错误码
			int sendMessageData(
				const std::string flagID,
				const std::string fromID,
				const std::string toID,
				const std::string msg);

			//客户端数据接收处理
			//@flagID : Request/Response标识
			//@fromID : 发送者ID标识
			//@toID : 接收者ID标识
			//@msg : 消息数据
			virtual void afterClientPolledMessageProcess(
				const std::string flagID,
				const std::string fromID,
				const std::string toID,
				const std::string msg) = 0;
			virtual const std::string buildAutoRegisterToBrokerMessage(void) = 0;

		protected:
			//创建客户端模型
			//@address : 服务端地址
			//@name : 服务端名称
			//@Return : 错误码
			virtual int createNewClientModule(
				const std::string address,
				const std::string name);

			//销毁客户端模型
			//@Return : 错误码
			virtual int destroyClientModule(void);
			
		protected:
			const ClientModuleType clientModuleType;
			void* clientModule;
			bool stopped;
		};//class AbstractClient
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_ABSTRACT_CLIENT_H
