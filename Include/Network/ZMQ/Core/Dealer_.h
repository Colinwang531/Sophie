//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-08-11
//		Description :					Dealer类
//
//		History:						Author									Date										Description
//										王科威									2020-08-11									创建
//										王科威									2020-12-28									1.规范名字空间
//																															2.重构类设计
//

#ifndef FRAMEWORK_NETWORK_ZMQ_DEALER_H
#define FRAMEWORK_NETWORK_ZMQ_DEALER_H

namespace framework
{
    namespace network
    {
		namespace zeromq
		{
			class Msg;

			class Dealer
			{
			public:
				Dealer(void);
				virtual ~Dealer(void);

			public:
				//绑定监听端口号
				//@s : Socket实例
				//@port : 监听端口号
				//@Return : 错误码
				int bind(
					void* s = nullptr,
					const unsigned short port = 0);

				//连接远程服务
				//@s : Socket实例
				//@ipv4 : 服务IPv4地址
				//@port : 服务端口号
				//@Return : 错误码
				int connect(
					void* s = nullptr, 
					const char* ipv4 = nullptr, 
					const unsigned short port = 0);

				//发送消息
				//@s : Socket实例
				//@msg : 消息数据
				//@Return : 错误码
				virtual int send(
					void* s = nullptr,
					Msg* msg = nullptr);
			};//class Dealer
		}//namespace zeromq
    }//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ZMQ_DEALER_H
