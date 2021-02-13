//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-06
//		Description :					TCP接收器类
//
//		History:						Author									Date										Description
//										王科威									2020-05-06									创建
//

#ifndef FRAMEWORK_NETWORK_ASIO_TCP_RECEIVER_H
#define FRAMEWORK_NETWORK_ASIO_TCP_RECEIVER_H

#include "boost/enable_shared_from_this.hpp"
#include "boost/function.hpp"
#include "boost/system/error_code.hpp"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			typedef boost::function<void(const int, const void*, const char*, const int)> AfterTCPReceiveResultCallback;

			class TCPReceiver 
				: public boost::enable_shared_from_this<TCPReceiver>
			{
			public:
				TCPReceiver(
					AfterTCPReceiveResultCallback callback = nullptr, 
					const int bufsize = 0);
				virtual ~TCPReceiver(void);

			public:
				//接收数据
				//@s : socket句柄
				//@Return : 错误码值
				virtual int receive(const void* s = nullptr);

			protected:
				void afterTCPReceiveResultNotification(
					const boost::system::error_code e,
					const void* s = nullptr,
					const int bytes = 0);

			private:
				AfterTCPReceiveResultCallback afterTCPReceiveResultCallback;
				char* buffer;
				const int bufferSize;
			};//class TCPReceiver
		}//namespace asio
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ASIO_TCP_RECEIVER_H
