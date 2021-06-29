//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-04
//		Description :					Pub类
//
//		History:						Author									Date										Description
//										王科威									2021-06-04									创建
//

#ifndef FRAMEWORK_LIBNETWORK_ZMQ_PUB_H
#define FRAMEWORK_LIBNETWORK_ZMQ_PUB_H

#include <string>

namespace framework
{
	namespace libnetwork
	{
		namespace zmq
		{
			class IPub;

			class Pub
			{
			public:
				//@c : 上下文实例
				Pub(void* c = nullptr);
				~Pub(void);

			public:
				//启动
				//ipv4 : IPv4地址
				//@port : 监听端口
				//@hwm : 缓存大小
				//@Return : 错误码
				int start(
					const std::string ipv4,
					const unsigned short port = 0,
					const int hwm = -1);

				//停止
				//@Return : 错误码
				int stop(void);

				//发送数据
				//@data : 数据内容
				//@Return : 错误码
				int send(const std::string data);

			private:
				IPub* pub;
			};//class Pub
		}//namespace zmq
	}//namespace libnetwork
}//namespace framework

#endif//FRAMEWORK_LIBNETWORK_ZMQ_PUB_H
