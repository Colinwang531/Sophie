#include "boost/checked_delete.hpp"
#include "boost/make_shared.hpp"
#include "Packet/Alarm/AlarmPacket.h"
using AlarmPacket = base::packet::AlarmPacket;
using AlarmPacketPtr = boost::shared_ptr<AlarmPacket>;
#include "Alarm/AbstractAlarm.h"
using AbstractAlarm = base::alarm::AbstractAlarm;
#ifdef WINDOWS
#include "Protocol/win/Message.pb.h"
#include "Protocol/win/Alarm.pb.h"
#else
#include "Protocol/linux/Message.pb.h"
#include "Protocol/linux/Alarm.pb.h"
#endif//WINDOWS
#include "Protocol/AlarmPhrase.h"

namespace base
{
	namespace protocol
	{
		AlarmParser::AlarmParser(){}
		AlarmParser::~AlarmParser(){}

		DataPacketPtr AlarmParser::parseMessage(void* a /* = nullptr */)
		{
			DataPacketPtr pkt{ boost::make_shared<AlarmPacket>() };

			if (pkt)
			{
				msg::Alarm* ma{ reinterpret_cast<msg::Alarm*>(a) };
				const msg::Alarm_Command command{ ma->command() };

				if (msg::Alarm_Command::Alarm_Command_NOTIFY == command)
				{
					AlarmPacketPtr alarmpkt{
						boost::dynamic_pointer_cast<AlarmPacket>(pkt) };
					const msg::AlarmInfo* info{ ma->release_alarminfo() };
					const std::string picture{ info->picture() }, cid{ info->cid() }, time{ info->time() }, uid{ info->uid() };
					const msg::AlarmPosition& pos{ info->alarmposition()[0] };

					alarmpkt->setMessagePacketCommand(static_cast<int>(info->type()));
					alarmpkt->setAlarmImage((unsigned char*)info->picture().c_str());
					alarmpkt->setStreamUrl(info->cid().c_str());
					alarmpkt->setAlarmClock(info->time().c_str());
					alarmpkt->setFaceID(atoi(info->uid().c_str()));
					alarmpkt->setAlarmRange(pos.x(), pos.y(), pos.w(), pos.h());
				}
			}

			return pkt;
		}

		AlarmPacker::AlarmPacker() {}
		AlarmPacker::~AlarmPacker() {}

		const std::string AlarmPacker::packMessage(DataPacketPtr pkt)
		{
			std::string msgstr;
			msg::MSG mm;
			mm.set_type(msg::MSG_Type::MSG_Type_ALARM);
			mm.set_sequence(pkt->getPacketSequence());
			mm.set_timestamp(pkt->getPacketTimestamp());
			msg::Alarm* c{ mm.mutable_alarm() };

			if (c)
			{
				AlarmPacketPtr alarmpkt{ 
					boost::dynamic_pointer_cast<AlarmPacket>(pkt) };

				c->set_command(msg::Alarm_Command::Alarm_Command_NOTIFY);
				msg::AlarmInfo* info{ c->mutable_alarminfo() };
				info->set_type(static_cast<msg::AlarmInfo_Type>(alarmpkt->getMessagePacketCommand()));
				info->set_cid(alarmpkt->getStreamUrl());
				info->set_time(alarmpkt->getAlarmClock());
				info->set_picture((const char*)alarmpkt->getAlarmImage());
				int x = 0, y = 0, w = 0, h = 0;
				alarmpkt->getAlarmRange(x, y, w, h);
				msg::AlarmPosition* pos{ info->add_alarmposition() };
				pos->set_x(x);
				pos->set_y(y);
				pos->set_w(w);
				pos->set_h(h);

				const int faceid{ alarmpkt->getFaceID() };
				char alarmFaceID[128]{ 0 };
#ifdef WINDOWS
				sprintf_s(alarmFaceID, 128, "%d", faceid);
#else
				sprintf(alarmFaceID, "%d", faceid);
#endif
				info->set_uid(alarmFaceID);
			}

			mm.SerializeToString(&msgstr);
			mm.release_component();
			return msgstr;
		}
	}//namespace protocol
}//namespace base
