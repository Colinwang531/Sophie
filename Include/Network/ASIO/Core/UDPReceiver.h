//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-12-04
//		Description :					UDP��������
//
//		History:						Author									Date										Description
//										������									2020-12-04									����
//

#ifndef FRAMEWORK_NETWORK_ASIO_UDP_RECEIVER_H
#define FRAMEWORK_NETWORK_ASIO_UDP_RECEIVER_H

#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/function.hpp"
#include "boost/system/error_code.hpp"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			typedef boost::function<void(void*, const int, const void*, const int, const char*, const unsigned short)> AfterUDPReceiveResultCallback;

			class UDPReceiver 
				: public boost::enable_shared_from_this<UDPReceiver>
			{
			public:
				UDPReceiver(
					AfterUDPReceiveResultCallback callback = nullptr,
					const int bufsize = 0);
				virtual ~UDPReceiver(void);

			public:
				//��������
				//@s : socket���
				//@Return : ������ֵ
				virtual int receive(const void* s = nullptr);

			protected:
				void afterUDPReceiveResultNotification(
					const boost::system::error_code e,
					const void* s = nullptr,
					const int bytes = 0);

			private:
				AfterUDPReceiveResultCallback afterUDPReceiveResultCallback;
				void* buffer;
				const int bufferSize;
				boost::asio::ip::udp::endpoint rep;
			};//class UDPReceiver
		}//namespace asio
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ASIO_UDP_RECEIVER_H
