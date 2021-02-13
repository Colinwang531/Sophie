//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-08-10
//		Description :					ZMQ消息类
//
//		History:						Author									Date										Description
//										王科威									2020-08-10									创建
//										王科威									2020-11-25									1.修改消息包抽象设计
//																															2.逻辑上实现数据队列的FILO模式
//

#ifndef FRAMEWORK_NETWORK_ZMQ_MESSAGE_H
#define FRAMEWORK_NETWORK_ZMQ_MESSAGE_H

// #include <vector>
// #include <string>

namespace framework
{
    namespace network
    {
		namespace zeromq
		{
			class Msg
			{
			public:
				Msg(void);
				~Msg(void);

			public:
				//添加消息段数据
				//@msg : 消息段数据
				//@Command : 1.从首部添加新数据
				//			 2.添加数据的过程是线程不安全的
				void addMessage(const std::string msg);

				//移除消息段数据
				//@Return : 被移除的消息段数据
				//@Command : 1.从首部移除数据
				//			 2.移除数据的过程是线程不安全的
				const std::string removeMessage(void);

				//接收消息数据
				//@s : SOCKET标识
				//@Return : 错误码
				int recvData(void* s = nullptr);

				//发送消息数据
				//@s : SOCKET标识
				//@Return : 错误码
				int sendData(void* s = nullptr);

			private:
				std::vector<std::string> msgDataGroup;
			};//class Msg
		}//namespace zeromq
    }//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ZMQ_MESSAGE_H
