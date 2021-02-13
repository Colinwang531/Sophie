//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-06
//		Description :					TCP发送器类
//
//		History:						Author									Date										Description
//										王科威									2020-05-06									创建
//										王科威									2020-12-30									重构类设计
//

#ifndef FRAMEWORK_NETWORK_ASIO_TCP_SENDER_H
#define FRAMEWORK_NETWORK_ASIO_TCP_SENDER_H

#include "boost/enable_shared_from_this.hpp"
#include "boost/function.hpp"
#include "boost/system/error_code.hpp"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			typedef boost::function<void(const int, const int)> AfterTCPSendResultCallback;

			class TCPSender 
				: public boost::enable_shared_from_this<TCPSender>
			{
			public:
				TCPSender(
					AfterTCPSendResultCallback callbak = nullptr);
				virtual ~TCPSender(void);

			public:
				virtual int send(
					const void* s = nullptr,
					const char* data = nullptr,
					const int bytes = 0);

			private:
				void afterTCPSendResultNotification(
					const boost::system::error_code e,
					const int bytes = 0);

			private:
				AfterTCPSendResultCallback afterTCPSendResultCallback;
			};//class TCPSender
		}//namespace asio
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ASIO_TCP_SENDER_H
