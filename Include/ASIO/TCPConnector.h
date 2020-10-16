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

#ifndef BASE_NETWORK_TCP_CONNECTOR_H
#define BASE_NETWORK_TCP_CONNECTOR_H

#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/function.hpp"
#include "boost/system/error_code.hpp"

namespace base
{
	namespace network
	{
		typedef boost::function<void(boost::asio::ip::tcp::socket*, const boost::system::error_code)> AfterGotRemoteConnectedNotificationCallback;

		class ASIOService;

		class TCPConnector : public boost::enable_shared_from_this<TCPConnector>
		{
		public:
			TCPConnector(
				ASIOService& service,
				AfterGotRemoteConnectedNotificationCallback callback = nullptr);
			virtual ~TCPConnector(void);

			//����TCP����
			//@address : ������IP��ַ
			//@port : �����˿ں�
			//@Return : ������ֵ
			int setConnect(
				const char* address = nullptr, 
				const unsigned short port = 60531);

		private:
			void afterRemoteConnectedNotificationCallback(
				boost::asio::ip::tcp::socket* s, 
				const boost::system::error_code e);

		private:
			ASIOService& asioService;
			AfterGotRemoteConnectedNotificationCallback afterGotRemoteConnectedNotificationCallback;
		};//class TCPConnector
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_TCP_CONNECTOR_H
