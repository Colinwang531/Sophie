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

#include "Protocol/CommandPhrase.h"

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

		class AlarmParser : public CommandParser
		{
		public:
			AlarmParser(void);
			~AlarmParser(void);

		public:
			void* parseAlarmMessage(void* msg = nullptr) override;
		};//class AlarmParser
	}//namespace protocol
}//namespace base

#endif//BASE_PROTOCOL_ALARM_PHRASE_H
