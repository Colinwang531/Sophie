#include "Error.h"
#include "output/Alarm.pb.h"
#include "Data/Protocol/ProtocolAlarm.h"

namespace framework
{
	namespace data
	{
		ProtocolAlarm::ProtocolAlarm() : Data()
		{}
		ProtocolAlarm::~ProtocolAlarm()
		{}

		int ProtocolAlarm::unpack(const std::string msg, AlarmMsg& am)
		{
			int e{ msg.empty() ? eInvalidParameter : eSuccess };

			if (eSuccess == e)
			{
				msg::AlarmMessage msg_;
				if (msg_.ParseFromArray(msg.c_str(), static_cast<int>(msg.length())))
				{
					//Command
					am.command = msg_.command();

					//Alarm
					if (msg_.has_alarm())
					{
						const msg::Alarm& alm{ msg_.alarm() };
						am.alarm.type = alm.type();
						am.alarm.camera = alm.camera();
						am.alarm.timestamp = alm.timestamp();
						am.alarm.picture = alm.picture();
						const msg::Range& range{ alm.ranges(0) };
						am.alarm.x = range.x(); 
						am.alarm.y = range.y();
						am.alarm.w = range.w();
						am.alarm.h = range.h();
						am.alarm.idforface = alm.idforface(); 
						am.alarm.inoutforface = alm.inoutforface(); 
						am.alarm.bodycount = alm.bodycount();
					}

					//pid
					if (msg_.has_pid())
					{
						am.pid = msg_.pid();
					}
				}
				else
				{
					e = eBadParseProtocol;
				}
			}

			return e;
		}

		const std::string ProtocolAlarm::pack(const AlarmMsg& am)
		{
			std::string str;
			msg::AlarmMessage almmsg;

			//Command
			almmsg.set_command(static_cast<msg::AlarmMessage_Command>(am.command));
			//pid
			almmsg.set_pid(am.pid);
			//Alarm
			msg::Alarm* alm{ almmsg.mutable_alarm() };
			alm->set_type(static_cast<msg::Alarm_Type>(am.alarm.type));
			alm->set_camera(am.alarm.camera);
			alm->set_timestamp(am.alarm.timestamp);
			alm->set_picture(am.alarm.picture);
			msg::Range* range{ alm->add_ranges() };
			range->set_x(am.alarm.x);
			range->set_y(am.alarm.y);
			range->set_w(am.alarm.w);
			range->set_h(am.alarm.h);
			alm->set_idforface(am.alarm.idforface);
			alm->set_inoutforface(am.alarm.inoutforface);
			alm->set_bodycount(am.alarm.bodycount);

			almmsg.SerializeToString(&str);
			return str;
		}
	}//namespace protocol
}//namespace base
