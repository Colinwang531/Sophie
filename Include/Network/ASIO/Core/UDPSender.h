//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-12-15
//		Description :					UDP��������
//
//		History:						Author									Date										Description
//										������									2020-12-15									����
//										������									2020-12-31									�ع������
//

#ifndef FRAMEWORK_NETWORK_ASIO_UDP_SENDER_H
#define FRAMEWORK_NETWORK_ASIO_UDP_SENDER_H

#include "boost/enable_shared_from_this.hpp"
#include "boost/function.hpp"
#include "boost/system/error_code.hpp"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			typedef boost::function<void(const int, const int)> AfterUDPSendResultCallback;

			class UDPSender 
				: public boost::enable_shared_from_this<UDPSender>
			{
			public:
				UDPSender(AfterUDPSendResultCallback callback = nullptr);
				virtual ~UDPSender(void);

			public:
				int send(
					const void* s = nullptr,
					const char* ipv4 = nullptr,
					const unsigned short port = 0,
					const void* data = nullptr,
					const int bytes = 0);

			private:
				void afterUDPSendResultNotification(
					const boost::system::error_code e,
					const int bytes = 0);

			private:
				AfterUDPSendResultCallback afterUDPSendResultCallback;
			};//class UDPSender
		}//namespace asio
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ASIO_UDP_SENDER_H
