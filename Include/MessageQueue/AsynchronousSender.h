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
			AsynchronousSender(void);
			virtual ~AsynchronousSender(void);

		public:
			//发送数据
			//@s : SOCKET句柄值
			//@data : 数据内容
			//@bytes : 数据字节数
			//@forcemore : 强制分包标识
			//             true == forcemore时data的数据无论大小都视为分包
			//@Return : 实际发送字节数
			const unsigned long long sendData(
				void* s = nullptr, 
				const void* data = nullptr, 
				const unsigned long long bytes = 0,
				const bool forcemore = false);

		private:
			//发送分包数据
			//@s : SOCKET句柄值
			//@data : 数据内容
			//@bytes : 数据字节数
			//@more : 数据分包结束标识,true标识还有数据待发送,false标识数据发送完成
			//@Return : 错误码值
			virtual const unsigned long long sendPartialData(
				void* s = nullptr, 
				const void* data = nullptr, 
				const unsigned long long bytes = 0, 
				const bool more = false);
		};//class AsynchronousSender
	}//namespace module
}//namespace mq

#endif//ASYNCHRONOUS_SENDER_H
