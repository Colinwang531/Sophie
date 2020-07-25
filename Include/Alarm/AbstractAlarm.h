//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-20
//		Description :					报警类
//
//		History:						Author									Date										Description
//										王科威									2020-07-20									创建
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
			//获取报警摄像机ID标识
			//@Return : 摄像机ID标识
			inline const std::string getAlarmCameraID(void) const
			{
				return cameraID;
			}

			//获取报警类型
			//@Return : 报警类型
			inline const AlarmType getAlarmType(void) const
			{
				return alarmType;
			}

			//读/写报警图片数据
			//@name : 报警图片
			//@Return : 报警图片
			inline void setAlarmPicture(const std::string picture)
			{
				alarmPicture = picture;
			}
			inline const std::string getAlarmPicture(void) const
			{
				return alarmPicture;
			}

			//读/写报警时间
			//@tm : 报警时间
			//@Return : 报警时间
			inline void setAlarmTime(const std::string tm)
			{
				alarmTime = tm;
			}
			inline const std::string getAlarmTime(void) const
			{
				return alarmTime;
			}

			//添加报警区域
			//@area : 报警区域
			inline void addAlarmArea(const AlarmArea area)
			{
				alarmAreas.push_back(area);
			}

			//获取报警人员ID标识
			//@Return : 人员ID标识
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
