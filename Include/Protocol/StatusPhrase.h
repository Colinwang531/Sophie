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

#include "boost/shared_ptr.hpp"
#include "Packet/DataPacket.h"
using DataPacketPtr = boost::shared_ptr<base::packet::DataPacket>;

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
			//将Protocol Buffers的组件实例转化为MessagePacket实例
			//@c : Protocol Buffers封装的Status实例
			//@Return : MessagePacket实例
			DataPacketPtr parseMessage(void* s = nullptr);
		};//class StatusParser

		class StatusPacker
		{
		public:
			StatusPacker(void);
			~StatusPacker(void);

		public:
			//将MessagePacket实例转换为Protocol buffers的序列化字符串
			//@pkt : MessagePacket实例
			//@Return : 序列化字符串
			void* packToStatusMessage(void* pkt = nullptr);
		};//class StatusPacker
	}//namespace protocol
}//namespace base

#endif//BASE_PROTOCOL_STATUS_PHRASE_H
