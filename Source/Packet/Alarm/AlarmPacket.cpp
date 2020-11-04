#include <string.h>
#include "boost/checked_delete.hpp"
#include "Error.h"
#include "Packet/Alarm/AlarmPacket.h"

namespace base
{
	namespace packet
	{
		AlarmPacket::AlarmPacket()
			: MessagePacket(MessagePacketType::MESSAGE_PACKET_TYPE_ALARM), 
			alarmImage{ nullptr }, streamUrl{ nullptr }, alarmClock{ nullptr }, faceID{ -1 }, 
			alarmX{ 0 }, alarmY{ 0 }, alarmW{ 0 }, alarmH{ 0 }
		{}

		AlarmPacket::~AlarmPacket()
		{
			boost::checked_array_delete(alarmImage);
			boost::checked_array_delete(streamUrl);
			boost::checked_array_delete(alarmClock);
		}

		void AlarmPacket::setAlarmImage(const unsigned char* image /* = nullptr */)
		{
			const std::size_t imageSize{ strlen((const char*)image) };
			boost::checked_array_delete(alarmImage);
			alarmImage = nullptr;

			alarmImage = new(std::nothrow) unsigned char[imageSize];
			if (alarmImage)
			{
#ifdef WINDOWS
				memcpy_s(alarmImage, imageSize, image, imageSize);
#else
				memcpy(alarmImage, image, imageSize);
#endif//WINDOWS
			}
		}

		void AlarmPacket::setStreamUrl(const char* url /* = nullptr */)
		{
			const std::size_t urlSize{ strlen((const char*)url) };
			boost::checked_array_delete(streamUrl);
			streamUrl = nullptr;

			streamUrl = new(std::nothrow) char[urlSize + 1];
			streamUrl[urlSize] = 0;
			if (streamUrl)
			{
#ifdef WINDOWS
				memcpy_s(streamUrl, urlSize, url, urlSize);
#else
				memcpy(streamUrl, url, urlSize);
#endif//WINDOWS
			}
		}

		void AlarmPacket::setAlarmClock(const char* clock /* = nullptr */)
		{
			const std::size_t clockSize{ strlen((const char*)clock) };
			boost::checked_array_delete(alarmClock);
			alarmClock = nullptr;

			alarmClock = new(std::nothrow) char[clockSize + 1];
			alarmClock[clockSize] = 0;
			if (alarmClock)
			{
#ifdef WINDOWS
				memcpy_s(alarmClock, clockSize, clock, clockSize);
#else
				memcpy(alarmClock, clock, clockSize);
#endif//WINDOWS
			}
		}
	}//namespace packet
}//namespace base
