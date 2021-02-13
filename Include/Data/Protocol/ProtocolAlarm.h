//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-29
//		Description :					����Э���װ/������
//
//		History:						Author									Date										Description
//										������									2020-05-29									����
//

#ifndef FRAMEWORK_DATA_PROTOCOL_ALARM_H
#define FRAMEWORK_DATA_PROTOCOL_ALARM_H

#include "Data/Data.h"

namespace framework
{
	namespace data
	{
		typedef struct tagAlarmData_t
		{
			bool inoutforface;
			int type;
			int bodycount;
			int x;
			int y;
			int w;
			int h;
			std::string camera;
			std::string timestamp;
			std::string picture;
			std::string idforface;
		}AlarmData;

		typedef struct tagAlarmMsg_t
		{
			int command;
			std::string pid;
			AlarmData alarm;
		}AlarmMsg;

		class ProtocolAlarm final : public Data
		{
		public:
			ProtocolAlarm(void);
			~ProtocolAlarm(void);

		public:
			int unpack(const std::string msg, AlarmMsg& am);
			const std::string pack(const AlarmMsg& am);
		};//class ProtocolAlarm
	}//namespace protocol
}//namespace framework

#endif//FRAMEWORK_DATA_PROTOCOL_ALARM_H
