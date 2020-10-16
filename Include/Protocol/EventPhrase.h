//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-04
//		Description :					状态协议封装/解析类
//
//		History:						Author									Date										Description
//										王科威									2020-10-04									创建
//

#ifndef BASE_PROTOCOL_EVENT_PHRASE_H
#define BASE_PROTOCOL_EVENT_PHRASE_H

#include "boost/shared_ptr.hpp"
#include "Packet/DataPacket.h"
using DataPacketPtr = boost::shared_ptr<base::packet::DataPacket>;

namespace base
{
	namespace protocol
	{
		typedef enum class tagEventCommand_t : int
		{
			EVENT_COMMAND_NONE = 0,
			EVENT_COMMAND_CAPTURE_JPEG_REQ = 1,
			EVENT_COMMAND_CAPTURE_JPEG_REP = 2,
		}EventCommand;

		class EventParser
		{
		public:
			EventParser(void);
			~EventParser(void);

		public:
			//将Protocol Buffers的组件实例转化为MessagePacket实例
			//@e : Protocol Buffers封装的Event实例
			//@Return : MessagePacket实例
			DataPacketPtr parseMessage(void* e = nullptr);
		};//class EventParser

		class EventPacker
		{
		public:
			EventPacker(void);
			~EventPacker(void);

		public:
			const std::string packMessage(DataPacketPtr pkt);
		};//class EventPacker
	}//namespace protocol
}//namespace base

#endif//BASE_PROTOCOL_EVENT_PHRASE_H
