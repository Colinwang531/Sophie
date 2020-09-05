//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-04-30
//		Description :					上游服务端抽象类
//
//		History:						Author									Date										Description
//										王科威									2020-04-30									创建
//										王科威									2020-08-18									1.定义为抽象基类
//										     									          									2.新增服务端类型标识
//

#ifndef BASE_NETWORK_UPSTREAM_SERVER_H
#define BASE_NETWORK_UPSTREAM_SERVER_H

#include "AbstractServer.h"
#include "AbstractClient.h"

namespace base
{
	namespace network
	{
		class AbstractUpstreamServer : public AbstractServer, public AbstractClient
		{
		public:
			AbstractUpstreamServer(
				const ServerModuleType server = ServerModuleType::SERVER_MODULE_TYPE_NONE,
				const ClientModuleType upstream = ClientModuleType::CLIENT_MODULE_TYPE_NONE,
				const std::string address = "tcp:\\127.0.0.1:61001");
			virtual ~AbstractUpstreamServer(void);

		protected:
			//创建本地服务端模型
			//@address : 本地监听地址
			//@Return : 错误码
			int createNewServerModule(const std::string address) override;

			//销毁本地服务端模型
			//@Return : 错误码
			int destroyServerModule(void) override;

		protected:
			const std::string upstreamAddress;
		};//class AbstractUpstreamServer
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_UPSTREAM_SERVER_H
