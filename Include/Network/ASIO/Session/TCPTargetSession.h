//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-06-15
//		Description :					TCP目的会话类
//
//		History:						Author									Date										Description
//										王科威									2020-05-13									创建
//

#ifndef FRAMEWORK_NETWORK_TCP_TARGET_SESSION_H
#define FRAMEWORK_NETWORK_TCP_TARGET_SESSION_H

#include "boost/enable_shared_from_this.hpp"
#include "boost/shared_ptr.hpp"
#include "TCPSession.h"
using TCPSession = framework::network::asio::TCPSession;
using TCPSessionPtr = boost::shared_ptr<TCPSession>;

namespace framework
{
	namespace network
	{
		namespace asio
		{
			//接收结果回调
			//@_1 : 错误码
			//@_2 : 接收数据缓存
			//@_3 : 接收数据大小
			typedef boost::function<void(const char*, const int, TCPSessionPtr)> AfterTargetTCPReceiveResultCallback;

			class TCPTargetSession 
				: public TCPSession, public boost::enable_shared_from_this<TCPTargetSession>
			{
			public:
				TCPTargetSession(
					boost::asio::ip::tcp::socket& so,
					AfterTargetTCPReceiveResultCallback ttrb = nullptr);
				virtual ~TCPTargetSession(void);

			private:
				void afterTCPSendResultCallback(
					const int e = 0,
					const int bytes = 0);
				void afterTCPReceiveResultCallback(
					const int e = 0,
					const void* data = nullptr,
					const int bytes = 0);

			private:
				AfterTargetTCPReceiveResultCallback ttr_callback;
			};//class TCPTargetSession
		}//namespace asio
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_TCP_TARGET_SESSION_H
