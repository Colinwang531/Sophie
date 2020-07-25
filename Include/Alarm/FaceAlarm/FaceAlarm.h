//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-20
//		Description :					����������
//
//		History:						Author									Date										Description
//										������									2020-07-20									����
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
			//��ȡ������ԱID��ʶ
			//@Return : ��ԱID��ʶ
			const std::string getAlarmPersonID(void) override;

		private:
			const std::string personID;
		};//class FaceAlarm
	}//namespace person
}//namespace base

#endif//BASE_ALARM_FACE_ALARM_H
