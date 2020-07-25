#include "Alarm/AbstractAlarm.h"

namespace base
{
	namespace alarm
	{
		AbstractAlarm::AbstractAlarm(
			const std::string cameraid, const AlarmType type /* = AlarmType::ALARM_TYPE_NONE */)
			: cameraID{ cameraid }, alarmType{ type }
		{}
		AbstractAlarm::~AbstractAlarm(){}

		const std::string AbstractAlarm::getAlarmPersonID()
		{
			return "";
		}
	}//namespace alarm
}//namespace base
