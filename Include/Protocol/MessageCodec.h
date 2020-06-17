//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-29
//		Description :					消息包协议封装/解析类
//
//		History:						Author									Date										Description
//										王科威									2020-05-15									创建
//										王科威									2020-06-05									拆分消息解析和封装类
//										王科威									2020-06-08									将消息属性和处理进行分离,该类只服务处理消息
//

#ifndef MESSAGE_CODEC_H
#define MESSAGE_CODEC_H

namespace base
{
	namespace protocol
	{
		class MessageParser
		{
		public:
			MessageParser(void);
			~MessageParser(void);

		public:
			//创建消息包
			//@Return : 消息包实例
			void* createNewPacket(void);

			//销毁消息包
			//@msg : 消息包实例
			//@Comment : 使用createNewPacket创建的消息包实例必须使用该方法进行销毁
			//			 调用者也可以自行进行销毁
			void destroyPacket(void* pkt = nullptr);

			//消息解析
			//@msg : 消息字节流
			//@pkt : 消息实例,由createNewMessage创建
			//@Return : 错误码
			int unpackMessage(
				const char* msg = nullptr, 
				void* pkt = nullptr);

		private:
			int parseAlarmMessage(void* msg = nullptr);
			int parseAlgorithmMessage(void* msg = nullptr);
			int parseComponentMessage(void* c = nullptr, void* pkt = nullptr);
			int parseCrewMessage(void* msg = nullptr);
			int parseDeviceMessage(void* msg = nullptr);
			int parseStatusMessage(void* s = nullptr, void* pkt = nullptr);
			int parseUserMessage(void* msg = nullptr);
		};//class MessageParser

		class MessagePacker
		{
		public:
			MessagePacker(void);
			~MessagePacker(void);

		public:
			//封装消息
			//@type : 消息类型
			//@sequence : 消息序列号
			//@data : 消息数据
			//@Return : 错误码
			const std::string packMessage(
				const int type = 0, 
				const long long sequence = 0,
				void* data = nullptr);
		};//class MessagePacker
	}//namespace protocol
}//namespace base

#endif//MESSAGE_CODEC_H
