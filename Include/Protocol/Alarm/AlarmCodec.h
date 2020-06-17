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

#ifndef ALARM_CODEC_H
#define ALARM_CODEC_H

#include <string>

namespace base
{
	namespace protocol
	{
		typedef enum class tagAlarmType_t : unsigned char
		{
			SLEEP = 1,
			FIGHT = 2,
			HELMET = 3,
			PHONE = 4,
			FACE = 5
		}AlarmType;//enum class tagAlarmType_t

		typedef struct tagAlarmPosition_t
		{
			int x;
			int y;
			int w;
			int h;
		}AlarmPosition;//struct tagAlarmPosition_t

		typedef struct tagAlarmInformation_t
		{
			AlarmType type;
			AlarmPosition position;
			int uid;
		}AlarmInformation;//struct tagAlarmInformation_t

		class AlarmCodec
		{
		public:
			AlarmCodec(void);
			virtual ~AlarmCodec(void);

		public:
			int unpackAlarm(void* msg = nullptr);

		private:
			AlarmInformation info;
			std::string cid;
			std::string time;
			std::string picture;
		};//class AlarmCodec
	}//namespace protocol
}//namespace base

#endif//ALARM_CODEC_H
