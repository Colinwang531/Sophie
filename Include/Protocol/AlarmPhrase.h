//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-29
//		Description :					报警协议封装/解析类
//
//		History:						Author									Date										Description
//										王科威									2020-05-29									创建
//

#ifndef BASE_PROTOCOL_ALARM_PHRASE_H
#define BASE_PROTOCOL_ALARM_PHRASE_H

#include "boost/shared_ptr.hpp"
#include "Packet/DataPacket.h"
using DataPacketPtr = boost::shared_ptr<base::packet::DataPacket>;

namespace base
{
	namespace protocol
	{
		typedef enum class tagAlarmType_t : int
		{
			ALARM_TYPE_NONE = 0,
			ALARM_TYPE_HELMET = 1,
			ALARM_TYPE_PHONE = 2,
			ALARM_TYPE_SLEEP = 3,
			ALARM_TYPE_FIGHT = 4,
			ALARM_TYPE_ATTENDANCE_IN = 5,
			ALARM_TYPE_ATTENDANCE_OUT = 6
		}AlarmType;

		class AlarmParser
		{
		public:
			AlarmParser(void);
			~AlarmParser(void);

		public:
			DataPacketPtr parseMessage(void* a = nullptr);
		};//class AlarmParser

		class AlarmPacker
		{
		public:
			AlarmPacker(void);
			~AlarmPacker(void);

		public:
			//封装算法消息
			//@command : 命令类型
			//@result : 错误码
			//@data : 消息数据
			//@Return : 消息内容
			const std::string packMessage(DataPacketPtr pkt);
		};//class AlarmPacker
	}//namespace protocol
}//namespace base

#endif//BASE_PROTOCOL_ALARM_PHRASE_H
