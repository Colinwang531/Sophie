//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-11
//		Description :					异步数据接收送类
//
//		History:						Author									Date										Description
//										王科威									2020-05-11									创建
//

#ifndef ASYNCHRONOUS_RECEIVER_H
#define ASYNCHRONOUS_RECEIVER_H

namespace mq
{
	namespace module
	{
		class AsynchronousReceiver
		{
		public:
			AsynchronousReceiver(void);
			virtual ~AsynchronousReceiver(void);

		public:
			//接收数据
			//@s : 数据接收的socket句柄值
			//@data : 数据内容
			//@bytes : 数据字节数
			//@Return : >0表示实际接收字节数,<0表示错误码值
			const int receiveData(
				void* s = nullptr, char* data = nullptr, const unsigned int bytes = 0);
		};//class AsynchronousReceiver
	}//namespace module
}//namespace mq

#endif//ASYNCHRONOUS_RECEIVER_H
