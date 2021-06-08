//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-04
//		Description :					ZMQ Dealer类
//
//		History:						Author									Date										Description
//										王科威									2021-06-04									创建
//

#ifndef FRAMEWORK_LIBNETWORK_ZMQ_DEALER_H
#define FRAMEWORK_LIBNETWORK_ZMQ_DEALER_H

namespace framework
{
    namespace libnetwork
    {
		namespace zmq
		{
			class Dealer
			{
			public:
				Dealer(void);
				virtual ~Dealer(void);

			public:
				//连接
				//ipv4 : IPv4地址
				//@port : 监听端口
				//@s : socket实例
				//@Return : 错误码
				virtual int connect(
					const std::string ipv4,
					const unsigned short port = 0,
					void* s = nullptr);

				//发送数据
				//@data : 数据内容
				//@s : socket实例
				//@Return : 错误码
				virtual int send(
					const std::string data,
					void* s = nullptr);
			};//class Dealer
		}//namespace zmq
    }//namespace libnetwork
}//namespace framework

#endif//FRAMEWORK_LIBNETWORK_ZMQ_DEALER_H
