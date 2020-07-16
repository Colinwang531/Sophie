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
//										王科威									2020-06-08									将消息属性和处理进行分离,只提供处理方法
//										王科威									2020-07-08									修改类名和方法名
//

#ifndef BASE_PROTOCOL_COMMAND_PHRASE_H
#define BASE_PROTOCOL_COMMAND_PHRASE_H

namespace base
{
	namespace protocol
	{
		class CommandParser
		{
		public:
			CommandParser(void);
			~CommandParser(void);

		public:
			//消息解析
			//@data : 字节流
			//@databytes : 字节流大小
			//@Return : AbstractPacket实例
			void* parseFromArray(
				const void* data = nullptr, 
				const unsigned int databytes = 0);

		private:
// 			int parseAlarmMessage(void* msg = nullptr);
 			void* parseAlgorithmMessage(void* msg = nullptr);
			void* parseComponentMessage(void* msg = nullptr);
// 			int parseCrewMessage(void* msg = nullptr);
 			void* parseDeviceMessage(void* msg = nullptr);
 			void* parseStatusMessage(void* msg = nullptr);
// 			int parseUserMessage(void* msg = nullptr);
		};//class CommandParser

		class CommandPacker
		{
		public:
			CommandPacker(void);
			~CommandPacker(void);

		public:
			//封装消息
			//@type : 消息类型
			//@sequence : 消息序列号
			//@data : 消息数据
			//@Return : 错误码
			const std::string packPacketToArray(void* pkt = nullptr);
		};//class CommandPacker
	}//namespace protocol
}//namespace base

#endif//BASE_PROTOCOL_COMMAND_PHRASE_H
