//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-12-14
//		Description :					UDP会话类
//
//		History:						Author									Date										Description
//										王科威									2020-12-14									创建
//

#ifndef FRAMEWORK_NETWORK_ASIO_UDP_SESSION_H
#define FRAMEWORK_NETWORK_ASIO_UDP_SESSION_H

#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"
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
			typedef boost::function<void(const int, const int)> AfterUDPSendResultCallback;

			//接收结果回调
			//@_1 : 错误码
			//@_2 : 接收数据缓存
			//@_3 : 接收数据大小
			//@_4 : 发送端IPv4地址
			//@_5 : 发送端端口号
			typedef boost::function<void(const int, const void*, const int, const char*, const unsigned short)> AfterUDPReceiveResultCallback;

			class UDPSession
				: public boost::enable_shared_from_this<UDPSession>
			{
			public:
				UDPSession(
					boost::asio::ip::udp::socket s,
					AfterUDPSendResultCallback scb = nullptr,
					AfterUDPReceiveResultCallback rcb = nullptr);
				virtual ~UDPSession(void);

			public:
				//发送数据
				//@s : socket句柄
				//@data : 数据
				//@bytes : 数据大小
				//@ipv4 : 接收端IPv4地址
				//@port : 接收端端口号
				//@Return : 错误码
				virtual int send(
					const void* data = nullptr,
					const int bytes = 0,
					const char* ipv4 = nullptr,
					const unsigned short port = 0);

				//接收数据
				//@Return : 错误码
				virtual int receive(void);

			private:
				//会话初始化
				void init(void);
				//会话反初始化
				void uninit(void);

			private:
				boost::asio::ip::udp::socket so;
				AfterUDPSendResultCallback s_callback;
				AfterUDPReceiveResultCallback r_callback;
				void* buffer;
				int bufsize;
			};//class UDPSession
		}//namespace asio
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ASIO_UDP_SESSION_H
