//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-06-15
//		Description :					TCPԴ�Ự��
//
//		History:						Author									Date										Description
//										������									2020-05-13									����
//

#ifndef FRAMEWORK_NETWORK_TCP_SOURCE_SESSION_H
#define FRAMEWORK_NETWORK_TCP_SOURCE_SESSION_H

#include "TCPSession.h"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			class TCPSourceSession : public TCPSession
			{
			public:
				TCPSourceSession(
					const std::string url,
					boost::asio::ip::tcp::socket& so,
					AfterTCPSendResultCallback scb = nullptr,
					AfterTCPReceiveResultCallback rcb = nullptr);
				virtual ~TCPSourceSession(void);

			public:
				int connect(
					const char* ipv4 = nullptr,
					const unsigned short port = 0);

			private:
				void afterTCPConnectResultCallback(const int e = 0);

			private:
				const std::string streamURL;
			};//class TCPSourceSession
		}//namespace asio
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_TCP_SOURCE_SESSION_H
