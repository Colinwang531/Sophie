//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-06
//		Description :					TCP会话类
//
//		History:						Author									Date										Description
//										王科威									2020-05-06									创建
//

#ifndef FRAMEWORK_NETWORK_ASIO_TCP_SESSION_H
#define FRAMEWORK_NETWORK_ASIO_TCP_SESSION_H

#include "boost/asio.hpp"
#include "boost/function.hpp"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			//发送结果回调
			//@_1 : 错误码
			//@_2 : 数据大小
			typedef boost::function<void(const int, const int)> AfterTCPSendResultCallback;

			//接收结果回调
			//@_1 : 错误码
			//@_2 : 接收数据缓存
			//@_3 : 接收数据大小
			typedef boost::function<void(const int, const void*, const int)> AfterTCPReceiveResultCallback;

			class TCPSession
			{
			public:
				TCPSession(
					boost::asio::ip::tcp::socket& s,
					AfterTCPSendResultCallback scb = nullptr,
					AfterTCPReceiveResultCallback rcb = nullptr);
				virtual ~TCPSession(void);

			public:
				//发送数据
				//@data : 数据
				//@bytes : 数据大小
				//@Return : 错误码
				virtual int send(
					const void* data = nullptr,
					const int bytes = 0);

				//接收数据
				//@Return : 错误码
				virtual int receive(void);

			private:
				//会话初始化
				void init(void);
				//会话反初始化
				void uninit(void);

			protected:
				boost::asio::ip::tcp::socket so;
				AfterTCPSendResultCallback s_callback;
				AfterTCPReceiveResultCallback r_callback;
				void* buffer;
				int bufsize;
			};//class TCPSession
		}//namespace asio
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ASIO_TCP_SESSION_H
