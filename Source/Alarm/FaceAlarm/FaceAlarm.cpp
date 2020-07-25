#include "Error.h"
#include "Alarm/FaceAlarm/FaceAlarm.h"

namespace base
{
	namespace alarm
	{
		FaceAlarm::FaceAlarm(
			const std::string personid,
			const std::string cameraid,
			const AlarmType type /* = AlarmType::ALARM_TYPE_ATTENDANCE_IN */)
			: AbstractAlarm(cameraid, type), personID{ personid }
		{}

		FaceAlarm::~FaceAlarm()
		{}

		const std::string FaceAlarm::getAlarmPersonID()
		{
			return personID;
		}
	}//namespace alarm
}//namespace base
