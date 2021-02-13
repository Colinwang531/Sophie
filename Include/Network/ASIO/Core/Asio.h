//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-06
//		Description :					ASIO类
//
//		History:						Author									Date										Description
//										王科威									2020-05-06									创建
//

#ifndef FRAMEWORK_NETWORK_ASIO_H
#define FRAMEWORK_NETWORK_ASIO_H

#include "boost/asio.hpp"
using IOContextGroup = std::vector<boost::asio::io_context>;
using IOWorkGroup = std::vector<boost::asio::io_service::work>;
#include "boost/thread.hpp"
#include "Mutex/RWLock.h"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			class ASIO
			{
			public:
				ASIO(void);
				virtual ~ASIO(void);

			public:
				//启动ASIO
				//@Return : 错误码值
				virtual int startASIO(void);

				//停止ASIO
				virtual int stopASIO(void);

				//获取TCP socket
				//@Return : TCP socket
				boost::asio::ip::tcp::socket getTCPSocket(void);

				//获取UDP socket
				//@port : 监听端口号
				//@Return : socket
				//@Comment : 0 < port时用于监听
				boost::asio::ip::udp::socket getUDPSocket(const unsigned short port = 0);

				//获取TCP监听
				//@port : 监听端口号
				//@Return : 监听
				//@Comment : 0 < port时用于监听
//				boost::asio::ip::tcp::acceptor getTCPListener(const unsigned short port = 0);

//			private:
				//获取IO
				//@Return : IO
				boost::asio::io_context& getIdleIO(void);

			private:
				IOContextGroup ioctxGroup;
				IOWorkGroup ioworkGroup;
				boost::thread_group iothreadGroup;
				int idleNumber;
				SharedMutex mtx;
			};//class ASIO
		}//namespace asio
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ASIO_H
