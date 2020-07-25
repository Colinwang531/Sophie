//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-20
//		Description :					人脸报警类
//
//		History:						Author									Date										Description
//										王科威									2020-07-20									创建
//

#ifndef BASE_ALARM_FACE_ALARM_H
#define BASE_ALARM_FACE_ALARM_H

#include "Alarm/AbstractAlarm.h"

namespace base
{
	namespace alarm
	{
		class FaceAlarm : public AbstractAlarm
		{
		public:
			FaceAlarm(
				const std::string personid,
				const std::string cameraid,
				const AlarmType type = AlarmType::ALARM_TYPE_ATTENDANCE_IN);
			virtual ~FaceAlarm(void);

		protected:
			//获取报警人员ID标识
			//@Return : 人员ID标识
			const std::string getAlarmPersonID(void) override;

		private:
			const std::string personID;
		};//class FaceAlarm
	}//namespace person
}//namespace base

#endif//BASE_ALARM_FACE_ALARM_H
