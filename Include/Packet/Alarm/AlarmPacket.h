//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-07
//		Description :					报警消息包类
//
//		History:						Author									Date										Description
//										王科威									2020-07-07									创建
//

#ifndef BASE_PACKET_ALARM_PACKET_H
#define BASE_PACKET_ALARM_PACKET_H

#include "Packet/Message/MessagePacket.h"

namespace base
{
	namespace packet
	{
		class AlarmPacket : public MessagePacket
		{
		public:
			AlarmPacket(void);
			virtual ~AlarmPacket(void);

		public:
			void setAlarmImage(const unsigned char* image = nullptr);
			inline const unsigned char* getAlarmImage(void) const
			{
				return alarmImage;
			}

			void setStreamUrl(const char* url = nullptr);
			inline const char* getStreamUrl(void) const
			{
				return streamUrl;
			}

			void setAlarmClock(const char* clock = nullptr);
			inline const char* getAlarmClock(void) const
			{
				return alarmClock;
			}

			inline void setFaceID(const int id = -1)
			{
				faceID = id;
			}
			inline const int getFaceID(void) const
			{
				return faceID;
			}

			inline void setAlarmRange(
				const int x = 0, const int y = 0, const int w = 0, const int h = 0)
			{
				alarmX = x;
				alarmY = y;
				alarmW = w;
				alarmH = h;
			}
			inline void getAlarmRange(int& x, int& y, int& w, int& h)
			{
				x = alarmX;
				y = alarmY;
				w = alarmW;
				h = alarmH;
			}

		private:
 			unsigned char* alarmImage;
			char* streamUrl;
			char* alarmClock;
			int faceID;
			int alarmX;
			int alarmY;
			int alarmW;
			int alarmH;
		};//class AlarmPacket
	}//namespace packet
}//namespace base

#endif//BASE_PACKET_ALARM_PACKET_H
