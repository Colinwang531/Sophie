//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-15
//		Description :					流媒体件客户端类
//
//		History:						Author									Date										Description
//										王科威									2020-09-15									创建
//

#ifndef BASE_NETWORK_MEDIA_STREAM_CLIENT_H
#define BASE_NETWORK_MEDIA_STREAM_CLIENT_H

#include "boost/shared_ptr.hpp"
#include "boost/system/system_error.hpp"
#include "DataStruct/Vector.h"
#include "DataStruct/UnorderedMap.h"
#include "ASIO/ASIOService.h"
using ASIOService = base::network::ASIOService;
#include "AbstractClient.h"

namespace base
{
	namespace network
	{
		class AbstractMediaStreamClient : public AbstractClient
		{
		public:
			AbstractMediaStreamClient(
				const std::string address, 
				const unsigned short port = 60531);
			virtual ~AbstractMediaStreamClient(void);

		public:
			//连接服务端
			//@did : 设备ID标识
			//@cid : 摄像机ID标识
			//@idx : 摄像机索引
			//@Return : 错误码
			int connectMediaServerWithID(
				const std::string did, 
				const std::string cid,
				const int idx = -1);

			//断开服务端连接
			//@did : 设备ID标识
			//@Return : 错误码
			//@Comment : 断开设备上所有的连接
			int disconnectMediaServer(const std::string did);

		protected:
			//创建客户端模型
			//@address : 服务端地址
			//@name : 服务端名称
			//@Return : 错误码
			int createNewClientModule(
				const std::string address,
				const std::string name) override;

			//销毁客户端模型
			//@Return : 错误码
			int destroyClientModule(void) override;

			virtual int createNewMediaStreamSession(
				const std::string url, 
				boost::asio::ip::tcp::socket* s) = 0;

		private:
			void afterGotRemoteConnectedNotificationCallback(
				boost::asio::ip::tcp::socket* s, const boost::system::error_code e);

		private:
			ASIOService asioService;
			Vector<std::string> urlGroup;
			const std::string mediaStreamIP;
			const unsigned short mediaStreamPort;
		};//class AbstractMediaStreamClient
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_MEDIA_STREAM_CLIENT_H
