//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-06
//		Description :					TCP监听器类
//
//		History:						Author									Date										Description
//										王科威									2020-05-06									创建
//										王科威									2020-12-31									重构类设计
//

#ifndef FRAMEWORK_NETWORK_ASIO_TCP_LISTENER_H
#define FRAMEWORK_NETWORK_ASIO_TCP_LISTENER_H

#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/function.hpp"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			//监听结果回调
			//@_1 : 错误码
			//@_2 : socket
			typedef boost::function<void(const int, boost::asio::ip::tcp::socket&)> AfterTCPAcceptResultCallback;

			class TCPListener 
				: public boost::enable_shared_from_this<TCPListener>
			{
			public:
				TCPListener(
					boost::asio::io_context& io,
					const unsigned short port = 0,
					AfterTCPAcceptResultCallback callback = nullptr);
				virtual ~TCPListener(void);

			public:
				//监听
				//@Return : 错误码值
				virtual int listen(void);

			private:
				boost::asio::ip::tcp::acceptor acceptor;
				AfterTCPAcceptResultCallback a_callbck;
			};//class TCPListener
		}//namespace asio
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ASIO_TCP_LISTENER_H
