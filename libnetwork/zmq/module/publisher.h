//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-01-15
//		Description :					Publisher抽象类
//
//		History:						Author									Date										Description
//										王科威									2021-01-15									创建
//


#ifndef FRAMEWORK_NETWORK_ZMQ_PUB_H
#define FRAMEWORK_NETWORK_ZMQ_PUB_H

namespace framework
{
	namespace network
	{
		namespace zeromq
		{
			class Msg;

			class Pub
			{
			public:
				Pub(void);
				virtual ~Pub(void);

			public:
				//启动Pub
				//@port : 监听端口号
				//@Return : 错误码
				virtual int startPub(const unsigned short port = 0);

				//停止Pub
				//@Return : 错误码
				virtual int stopPub(void);

				//发送消息
				//@msg : 消息数据
				//@Return : 错误码
				int sendMsg(Msg* msg = nullptr);

			protected:
				void* so;
			};//class Pub
		}//namespace zeromq
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ZMQ_PUB_H
