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

#ifndef BASE_PROTOCOL_STATUS_PHRASE_H
#define BASE_PROTOCOL_STATUS_PHRASE_H

namespace base
{
	namespace protocol
	{
		typedef enum class tagStatusCommand_t : int
		{
			STATUS_COMMAND_NONE = 0,
			STATUS_COMMAND_SET_REQ = 1,
			STATUS_COMMAND_SET_REP = 2,
			STATUS_COMMAND_QUERY_REQ = 3,
			STATUS_COMMAND_QUERY_REP = 3,
		}StatusCommand;

		typedef enum class tagStatusType_t : int
		{
			STATUS_TYPE_NONE = 0,
			STATUS_TYPE_SAIL = 1,
			STATUS_TYPE_AUTO = 2,
			STATUS_TYPE_NAME = 3
		}StatusType;

		class StatusParser
		{
		public:
			StatusParser(void);
			~StatusParser(void);

		public:
			//将Protocol Buffers的状态实例转化为AbstractPacket实例
			//@s : 通过Protocol Buffers解析得到的Status实例
			//@Return : AbstractPacket实例
			void* unpackFromStatusMessage(void* s = nullptr);
		};//class StatusParser

		class StatusPacker
		{
		public:
			StatusPacker(void);
			~StatusPacker(void);

		public:
			//封装状态消息
			//@command : 命令类型
			//@result : 当COMPONENT_COMMAND_*_REP == command时该参数有效,表示应答状态
			//@data : 消息数据
			//		  当COMPONENT_COMMAND_SIGNIN_REP == command时表示组件ID标识
			//		  当COMPONENT_COMMAND_QUERY_REP == command时表示组件信息集合
			//@Return : 消息内容
			void* packStatus(
				const int command = 0,
				const int result = 0,
				void* data = nullptr);
		};//class StatusPacker
	}//namespace protocol
}//namespace base

#endif//BASE_PROTOCOL_STATUS_PHRASE_H
