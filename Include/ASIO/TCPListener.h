//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-06
//		Description :					ASIO监听器类
//
//		History:						Author									Date										Description
//										王科威									2020-05-06									创建
//

#ifndef BASE_NETWORK_TCP_LISTENER_H
#define BASE_NETWORK_TCP_LISTENER_H

#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/function.hpp"

namespace base
{
	namespace network
	{
		typedef boost::function<void(boost::asio::ip::tcp::socket*, boost::system::error_code)> AfterGotRemoteConnectedNotificationCallback;

		class ASIOService;

		class TCPListener : public boost::enable_shared_from_this<TCPListener>
		{
		public:
			TCPListener(
				ASIOService& service, 
				AfterGotRemoteConnectedNotificationCallback callback = nullptr);
			virtual ~TCPListener(void);

		public:
			//启动TCP监听
			//@port : 监听端口号
			//@Return : 错误码值
			int setListen(const unsigned short port = 60531);

		private:
			void afterRemoteConnectedtNotificationCallback(
				boost::asio::ip::tcp::socket* s,
				boost::shared_ptr<boost::asio::ip::tcp::acceptor> ap,
				boost::system::error_code e);

		private:
			ASIOService& asioService;
			AfterGotRemoteConnectedNotificationCallback afterGotRemoteConnectedNotificationCallback;
		};//class TCPListener
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_TCP_LISTENER_H
