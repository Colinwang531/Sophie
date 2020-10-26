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

namespace base
{
	namespace network
	{
		class AbstractServer
		{
		public:
			AbstractServer(void);
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

			//发送数据
			//@commID : 通信ID标识
			//@roleID : 角色ID标识
			//@flagID : 标志ID标识
			//@fromID : 发送者ID标识
			//@toID : 接收者ID标识
			//@data : 消息数据
			//@Return : 错误码
			virtual int sendData(
				const std::string commID,
				const std::string roleID,
				const std::string flagID,
				const std::string fromID,
				const std::string toID,
				const std::string data) = 0;

			//客户端在线/离线超时处理
			virtual void afterAutoClientConnectionTimeoutProcess(void) = 0;

		protected:
			//创建本地服务端
			//@address : 本地监听地址
			//@Return : 错误码
			virtual int createNewServer(
				const std::string address) = 0;

			//销毁本地服务端
			//@Return : 错误码
			virtual int destroyServer(void) = 0;

		private:
			//客户端在线/离线检测线程
			void autoCheckClientConnectionTimeoutThread(void);

		protected:
			bool stopped;
		};//class AbstractServer
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_ABSTRACT_SERVER_H
