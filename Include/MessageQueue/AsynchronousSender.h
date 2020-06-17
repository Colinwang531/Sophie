//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-11
//		Description :					异步数据发送类
//
//		History:						Author									Date										Description
//										王科威									2020-05-11									创建
//

#ifndef ASYNCHRONOUS_SENDER_H
#define ASYNCHRONOUS_SENDER_H

namespace mq
{
	namespace module
	{
		class AsynchronousSender
		{
		public:
			AsynchronousSender(const unsigned int bytes = mtu);
			virtual ~AsynchronousSender(void);

		public:
			//发送数据
			//@s : 数据发送的socket句柄值
			//@data : 数据内容
			//@bytes : 数据字节数
			//@more : 数据发送结束标识,true标识还有数据待发送,false标识数据发送完成
			//@Return : 实际发送字节数
			const unsigned int sendData(
				void* s = nullptr, const char* data = nullptr, const unsigned int bytes = 0, const bool more = false);

		private:
			//发送拆包数据
			//@s : 数据发送的socket句柄值
			//@data : 数据内容
			//@bytes : 数据字节数
			//@more : 数据发送结束标识,true标识还有数据待发送,false标识数据发送完成
			//@Return : 错误码值
			virtual const unsigned int sendDisassemblyData(
				void* s = nullptr, const char* data = nullptr, const unsigned int bytes = 0, const bool more = false);

		private:
			const unsigned int disassemblyDataBytes;
			static const unsigned int mtu = 512;
		};//class MQSender
	}//namespace module
}//namespace mq

#endif//ASYNCHRONOUS_SENDER_H
