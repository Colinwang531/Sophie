//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-29
//		Description :					成员协议封装/解析类
//
//		History:						Author									Date										Description
//										王科威									2020-05-29									创建
//

#ifndef BASE_PROTOCOL_CREW_PHRASE_H
#define BASE_PROTOCOL_CREW_PHRASE_H

#include "boost/shared_ptr.hpp"
#include "Packet/DataPacket.h"
using DataPacketPtr = boost::shared_ptr<base::packet::DataPacket>;

namespace base
{
	namespace protocol
	{
		typedef enum class tagCrewCommand_t : int
		{
			CREW_COMMAND_NONE = 0,
			CREW_COMMAND_NEW_REQ = 1,
			CREW_COMMAND_NEW_REP = 2,
			CREW_COMMAND_DELETE_REQ = 3,
			CREW_COMMAND_DELETE_REP = 4,
			CREW_COMMAND_MODIFY_REQ = 5,
			CREW_COMMAND_MODIFY_REP = 6,
			CREW_COMMAND_QUERY_REQ = 7,
			CREW_COMMAND_QUERY_REP = 8
		}CrewCommand;

		class CrewParser
		{
		public:
			CrewParser(void);
			~CrewParser(void);

		public:
			//将Protocol Buffers的组件实例转化为MessagePacket实例
			//@c : Protocol Buffers封装的Crew实例
			//@Return : MessagePacket实例
			DataPacketPtr parseMessage(void* c = nullptr);
		};//class CrewParser

		class CrewPacker
		{
		public:
			CrewPacker(void);
			~CrewPacker(void);

		public:
			//将DataPacket实例转换为Protocol buffers的序列化字符串
			//@pkt : DataPacket实例
			//@Return : 序列化字符串
			const std::string packMessage(DataPacketPtr pkt);
		};//class CrewPacker
	}//namespace protocol
}//namespace base

#endif//BASE_PROTOCOL_CREW_PHRASE_H
