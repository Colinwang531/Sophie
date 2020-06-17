//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-29
//		Description :					状态协议封装/解析类
//
//		History:						Author									Date										Description
//										王科威									2020-05-29									创建
//										王科威									2020-06-12									添加数据结构和解析/封装方法
//

#ifndef STATUS_CODEC_H
#define STATUS_CODEC_H

namespace base
{
	namespace protocol
	{
		class StatusParser
		{
		public:
			StatusParser(void);
			~StatusParser(void);

		public:
			//创建状态消息包
			//@Return : 消息包实例
			void* createNewStatus(void);

			//销毁状态消息包
			//@pkt : 状态消息包实例
			//@Comment : 使用createNewStatus创建的状态消息包实例必须使用该方法进行销毁
			//			 调用者也可以自行进行销毁
			void destroyStatus(void* pkt = nullptr);

			//解析状态消息
			//@c : 状态消息数据
			//@pkt : 消息包
			//@Return : 错误码
			int unpackStatus(
				void* s = nullptr, 
				void* pkt = nullptr);
		};//class StatusParser
	}//namespace protocol
}//namespace base

#endif//STATUS_CODEC_H
