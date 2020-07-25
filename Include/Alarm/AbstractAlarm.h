//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-20
//		Description :					������
//
//		History:						Author									Date										Description
//										������									2020-07-20									����
//

#ifndef BASE_ALARM_ABSTRACT_ALARM_H
#define BASE_ALARM_ABSTRACT_ALARM_H

#include <string>
#include <vector>

namespace base
{
	namespace alarm
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

		typedef struct tagAlarmArea_t
		{
			int positionX;
			int positionY;
			int positionW;
			int positionH;
		}AlarmArea;

		class AbstractAlarm
		{
		public:
			AbstractAlarm(
				const std::string cameraid,
				const AlarmType type = AlarmType::ALARM_TYPE_NONE);
			virtual ~AbstractAlarm(void);

		public:
			//��ȡ���������ID��ʶ
			//@Return : �����ID��ʶ
			inline const std::string getAlarmCameraID(void) const
			{
				return cameraID;
			}

			//��ȡ��������
			//@Return : ��������
			inline const AlarmType getAlarmType(void) const
			{
				return alarmType;
			}

			//��/д����ͼƬ����
			//@name : ����ͼƬ
			//@Return : ����ͼƬ
			inline void setAlarmPicture(const std::string picture)
			{
				alarmPicture = picture;
			}
			inline const std::string getAlarmPicture(void) const
			{
				return alarmPicture;
			}

			//��/д����ʱ��
			//@tm : ����ʱ��
			//@Return : ����ʱ��
			inline void setAlarmTime(const std::string tm)
			{
				alarmTime = tm;
			}
			inline const std::string getAlarmTime(void) const
			{
				return alarmTime;
			}

			//��ӱ�������
			//@area : ��������
			inline void addAlarmArea(const AlarmArea area)
			{
				alarmAreas.push_back(area);
			}

			//��ȡ������ԱID��ʶ
			//@Return : ��ԱID��ʶ
			virtual const std::string getAlarmPersonID(void);

		private:
			const AlarmType alarmType;
			std::vector<AlarmArea> alarmAreas;
			std::string alarmPicture;
			std::string alarmTime;
			std::string cameraID;
		};//class AbstractAlarm
	}//namespace alarm
}//namespace base

#endif//BASE_ALARM_ABSTRACT_ALARM_H
