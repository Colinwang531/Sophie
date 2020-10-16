//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-06
//		Description :					ASIO��������
//
//		History:						Author									Date										Description
//										������									2020-05-06									����
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
			//����TCP����
			//@port : �����˿ں�
			//@Return : ������ֵ
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
