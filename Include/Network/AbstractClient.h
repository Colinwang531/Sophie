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

#include <string>

namespace base
{
	namespace network
	{
		class AbstractClient
		{
		public:
			AbstractClient(void);
			virtual ~AbstractClient(void);

		public:
			//启动客户端
			//@address : 服务端连接地址
			//@Return : 错误码
			int startClient(const std::string address);

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

			//发送数据
			//@roleID : 角色ID标识
			//@flagID : 标志ID标识
			//@fromID : 发送者ID标识
			//@toID : 接收者ID标识
			//@data : 消息数据
			//@Return : 错误码
			virtual int sendData(
				const std::string roleID,
				const std::string flagID,
				const std::string fromID,
				const std::string toID,
				const std::string data) = 0;

		protected:
			//创建客户端
			//@address : 服务端地址
			//@Return : 错误码
			virtual int createNewClient(
				const std::string address) = 0;

			//销毁客户端
			//@Return : 错误码
			virtual int destroyClient(void) = 0;
			
		protected:
			bool stopped;
		};//class AbstractClient
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_ABSTRACT_CLIENT_H
