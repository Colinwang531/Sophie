//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-07
//		Description :					ZMQ消息类
//
//		History:						Author									Date										Description
//										王科威									2021-06-07									创建
//

#ifndef FRAMEWORK_LIBNETWORK_ZMQ_MSG_H
#define FRAMEWORK_LIBNETWORK_ZMQ_MSG_H

namespace framework
{
    namespace libnetwork
    {
		namespace zmq
		{
			class Impl;

			class Msg
			{
			public:
				Msg(void);
				~Msg(void);

			public:
				//添加消息数据
				//@msg : 消息数据
				//@Comment : 尾部添加
				void add(const std::string msg);

				//删除消息数据
				//@Return : 消息数据
				//@Comment : 首部删除
				const std::string remove(void);

				//接收数据
				//@s : socket实例
				//@Return : 错误码
				int receive(void* s = nullptr);

				//发送数据
				//@s : socket实例
				//@Return : 错误码
				int send(void* s = nullptr);

			private:
				Impl* impl;
			};//class Msg
		}//namespace zeromq
    }//namespace network
}//namespace framework

#endif//FRAMEWORK_LIBNETWORK_ZMQ_MSG_H
