//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-06
//		Description :					TCP��������
//
//		History:						Author									Date										Description
//										������									2020-05-06									����
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
				//��������
				//@s : socket���
				//@Return : ������ֵ
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
