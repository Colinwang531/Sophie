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

#include "Protocol/CommandPhrase.h"

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

		class ComponentParser : public CommandParser
		{
		public:
			ComponentParser(void);
			~ComponentParser(void);

		public:
			void* parseComponentMessage(void* msg = nullptr) override;
		};//class ComponentParser

		class ComponentPacker
		{
		public:
			ComponentPacker(void);
			~ComponentPacker(void);

		public:
			//封装组件消息
			//@command : 命令类型
			//@result : 当COMPONENT_COMMAND_*_REP == command时该参数有效,表示应答状态
			//@data : 消息数据
			//		  当COMPONENT_COMMAND_SIGNIN_REP == command时表示组件ID标识
			//		  当COMPONENT_COMMAND_QUERY_REP == command时表示组件信息集合
			//@Return : 消息内容
			void* packToComponentMessage(
				const int command = 0,
				const int result = 0,
				const void* data = nullptr);
		};//class ComponentPacker
	}//namespace protocol
}//namespace base

#endif//BASE_PROTOCOL_COMPONENT_PHRASE_H
