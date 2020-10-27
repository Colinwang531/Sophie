#include "boost/checked_delete.hpp"
#include "boost/make_shared.hpp"
#include "Packet/Message/MessagePacket.h"
using MessagePacket = base::packet::MessagePacket;
using MessagePacketPtr = boost::shared_ptr<MessagePacket>;
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
			DataPacketPtr pkt{
				boost::make_shared<MessagePacket>(
					base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_ALARM) };

			if (pkt)
			{
				msg::Alarm* ma{ reinterpret_cast<msg::Alarm*>(a) };
				const msg::Alarm_Command command{ ma->command() };
				MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
				msgpkt->setMessagePacketCommand(static_cast<int>(command));

// 				if (msg::Alarm_Command::Alarm_Command_NOTIFY == command)
// 				{
// 					const msg::AlarmInfo* info{ ma->release_alarminfo() };
// 					const std::string picture{ info->picture() }, cid{ info->cid() }, time{ info->time() }, uid{ info->uid() };
// 					const msg::AlarmPosition& pos{ info->alarmposition() };
// 					const int x{ pos.x() }, y{ pos.y() }, w{ pos.w() }, h{ pos.h() };
// 
// 					char* pictureBuffer{ new(std::nothrow) char[picture.length()] };
// 					char* cidBuffer{ new(std::nothrow) char[cid.length()] };
// 					char* timeBuffer{ new(std::nothrow) char[time.length()] };
// 					char* uidBuffer{ new(std::nothrow) char[uid.length()] };
// 					int* xBuffer{ new(std::nothrow) int };
// 					int* yBuffer{ new(std::nothrow) int };
// 					int* wBuffer{ new(std::nothrow) int };
// 					int* hBuffer{ new(std::nothrow) int };
// 
// 					pkt->setPacketData((void*).c_str());
// 					pkt->setPacketData((void*)info->cid().c_str());
// 					pkt->setPacketData((void*)info->time().c_str());
// 					pkt->setPacketData((void*)&pos.x());
// 				}
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
				MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
				c->set_command(msg::Alarm_Command::Alarm_Command_NOTIFY);
				msg::AlarmInfo* info{ c->mutable_alarminfo() };
				info->set_type(static_cast<msg::AlarmInfo_Type>(msgpkt->getMessagePacketCommand()));
				info->set_picture(reinterpret_cast<const char*>(pkt->getPacketData(0)));
				info->set_cid(reinterpret_cast<const char*>(pkt->getPacketData(1)));
				info->set_time(reinterpret_cast<const char*>(pkt->getPacketData(2)));
				msg::AlarmPosition* pos{ info->add_alarmposition() };
				pos->set_x(*(reinterpret_cast<int*>(pkt->getPacketData(3))));
				pos->set_y(*(reinterpret_cast<int*>(pkt->getPacketData(4))));
				pos->set_w(*(reinterpret_cast<int*>(pkt->getPacketData(5))));
				pos->set_h(*(reinterpret_cast<int*>(pkt->getPacketData(6))));
				info->set_uid(reinterpret_cast<const char*>(pkt->getPacketData(7)));
			}

			mm.SerializeToString(&msgstr);
			mm.release_component();
			return msgstr;
		}
	}//namespace protocol
}//namespace base
