//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-04
//		Description :					Dealer类
//
//		History:						Author									Date										Description
//										王科威									2021-06-04									创建
//

#ifndef FRAMEWORK_LIBNETWORK_ZMQ_DEALER_H
#define FRAMEWORK_LIBNETWORK_ZMQ_DEALER_H

#include <string>

namespace framework
{
    namespace libnetwork
    {
		namespace zmq
		{
			typedef void (*DealerPollDataHandler)(const std::string, void*);
			class IDealer;

			class Dealer
			{
			public:
				//@c : 上下文实例
				//@handler : 数据读取回调
				//@param : 用户数据
				Dealer(
					void* c = nullptr,
					DealerPollDataHandler handler = nullptr,
					void* param = nullptr);
				~Dealer(void);

			public:
				//启动
				//@sourceID : 源ID标识
				//ipv4 : IPv4地址
				//@port : 监听端口
				//@Return : 错误码
				int start(
					const std::string sourceID,
					const std::string ipv4,
					const unsigned short port = 0);

				//停止
				//@Return : 错误码
				int stop(void);

				//发送数据
				//@data : 数据内容
				//@Return : 错误码
				int send(const std::string data);

			private:
				IDealer* dealer;
			};//class Dealer
		}//namespace zmq
    }//namespace libnetwork
}//namespace framework

#endif//FRAMEWORK_LIBNETWORK_ZMQ_DEALER_H
