//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-04
//		Description :					ZMQ Router类
//
//		History:						Author									Date										Description
//										王科威									2021-06-04									创建
//

#ifndef FRAMEWORK_LIBNETWORK_ZMQ_ROUTER_H
#define FRAMEWORK_LIBNETWORK_ZMQ_ROUTER_H

namespace framework
{
    namespace libnetwork
    {
		namespace zmq
		{
			class Router
			{
			public:
				Router(void);
				virtual ~Router(void);

			public:
				//绑定监听端口
				//ipv4 : IPv4地址
				//@port : 监听端口
				//@s : socket实例
				//@Return : 错误码
				//@Comment : 如果IPv4地址为空则监听所有地址的端口
				virtual int bind(
					const std::string ipv4,
					const unsigned short port = 0,
					void* s = nullptr);

				//发送数据
				//@targetID : 接收端ID标识
				//@data : 数据内容
				//@s : socket实例
				//@Return : 错误码
				virtual int send(
					const std::string targetID,
					const std::string data,
					void* s = nullptr);
			};//class Router
		}//namespace zmq
    }//namespace network
}//namespace framework

#endif//FRAMEWORK_LIBNETWORK_ZMQ_ROUTER_H
