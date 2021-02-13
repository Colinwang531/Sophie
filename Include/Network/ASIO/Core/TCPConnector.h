//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-06
//		Description :					TCP连接器类
//
//		History:						Author									Date										Description
//										王科威									2020-05-06									创建
//

#ifndef FRAMEWORK_NETWORK_ASIO_TCP_CONNECTOR_H
#define FRAMEWORK_NETWORK_ASIO_TCP_CONNECTOR_H

#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/function.hpp"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			typedef boost::function<void(const int)> AfterTCPConnectResultCallback;

			class TCPConnector 
				: public boost::enable_shared_from_this<TCPConnector>
			{
			public:
				TCPConnector(
					AfterTCPConnectResultCallback callback = nullptr);
				virtual ~TCPConnector(void);

			public:
				//连接
				//@so : Socket
				//@ipv4 : IPv4地址
				//@port : 端口号
				//@Return : 错误码
				virtual int connect(
					boost::asio::ip::tcp::socket& so,
					const char* ipv4 = nullptr,
					const unsigned short port = 0);

			private:
				AfterTCPConnectResultCallback c_callback;
			};//class TCPConnector
		}//namespace asio
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ASIO_TCP_CONNECTOR_H
