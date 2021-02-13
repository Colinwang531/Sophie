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
//										������									2020-12-31									�ع������
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
			//��������ص�
			//@_1 : ������
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
				//����
				//@Return : ������ֵ
				virtual int listen(void);

			private:
				boost::asio::ip::tcp::acceptor acceptor;
				AfterTCPAcceptResultCallback a_callbck;
			};//class TCPListener
		}//namespace asio
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ASIO_TCP_LISTENER_H
