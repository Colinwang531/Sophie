//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-15
//		Description :					组件协议封装/解析类
//
//		History:						Author									Date										Description
//										王科威									2020-05-15									创建
//										王科威									2020-05-29									添加数据结构和解析/封装方法
//

#ifndef BASE_PROTOCOL_COMPONENT_PHRASE_H
#define BASE_PROTOCOL_COMPONENT_PHRASE_H

#include "boost/shared_ptr.hpp"
#include "Packet/DataPacket.h"
using DataPacketPtr = boost::shared_ptr<base::packet::DataPacket>;

namespace base
{
	namespace protocol
	{
		typedef enum class tagComponentCommand_t : int
		{
			COMPONENT_COMMAND_NONE = 0,
			COMPONENT_COMMAND_SIGNIN_REQ = 1,
			COMPONENT_COMMAND_SIGNIN_REP = 2,
			COMPONENT_COMMAND_SIGNOUT_REQ = 3,
			COMPONENT_COMMAND_SIGNOUT_REP = 4,
			COMPONENT_COMMAND_QUERY_REQ = 5,
			COMPONENT_COMMAND_QUERY_REP = 6
		}ComponentCommand;

		class ComponentParser
		{
		public:
			ComponentParser(void);
			~ComponentParser(void);

		public:
			//将Protocol Buffers的组件实例转化为MessagePacket实例
			//@c : Protocol Buffers封装的Component实例
			//@Return : MessagePacket实例
			DataPacketPtr parseMessage(void* c = nullptr);
		};//class ComponentParser

		class ComponentPacker
		{
		public:
			ComponentPacker(void);
			~ComponentPacker(void);

		public:
			//将DataPacket实例转换为Protocol buffers的序列化字符串
			//@pkt : DataPacket实例
			//@Return : 序列化字符串
			const std::string packMessage(DataPacketPtr pkt);
		};//class ComponentPacker
	}//namespace protocol
}//namespace base

#endif//BASE_PROTOCOL_COMPONENT_PHRASE_H
