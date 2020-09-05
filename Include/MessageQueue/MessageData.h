//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-08-10
//		Description :					MQ消息类
//
//		History:						Author									Date										Description
//										王科威									2020-08-10									创建
//

#ifndef MQ_MESSAGE_MESSAGE_DATA_H
#define MQ_MESSAGE_MESSAGE_DATA_H

#include <vector>
#include <string>
#include "boost/noncopyable.hpp"
#include "socket_base.hpp"

namespace mq
{
    namespace message
    {
		class MessageData : private boost::noncopyable
		{
		public:
			MessageData(void);
			~MessageData(void);

		public:
			//接收消息数据
			//@s : SOCKET标识
			//@Return : 错误码
			int recvData(zmq::socket_base_t* s = nullptr);

			//发送消息数据
			//@s : SOCKET标识
			//@Return : 错误码
			int sendData(zmq::socket_base_t* s = nullptr);

			//获取消息数据个数
			//@Return : 消息数据个数
			inline const unsigned int getDataCount(void) const
			{
				return static_cast<unsigned int>(dataGroup.size());
			}

			//在首部添加消息数据
			//@data : 数据内容
			//@Return : 错误码
			int pushFront(const std::string data);

			//在尾部添加消息数据
			//@data : 数据内容
			//@Return : 错误码
			int pushEnd(const std::string data);

			//在首部删除消息数据
			//@Return : 数据内容
			const std::string popFront(void);

		private:
			//删除消息的所有数据
			void clearup(void);

		private:
			std::vector<std::string> dataGroup;
		};//class MessageData
    }//namespace message
}//namespace mq

#endif//MQ_MESSAGE_MESSAGE_DATA_H
