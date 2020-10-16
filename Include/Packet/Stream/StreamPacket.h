//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-17
//		Description :					流数据包类
//
//		History:						Author									Date										Description
//										王科威									2020-09-17									创建
//

#ifndef BASE_PACKET_STREAM_PACKET_H
#define BASE_PACKET_STREAM_PACKET_H

#include "Packet/DataPacket.h"

namespace base
{
	namespace packet
	{
		typedef enum tagStreamPacketType_t : int
		{
			STREAM_PACKET_TYPE_NONE = 0,
			STREAM_PACKET_TYPE_PRIVATE_HK,
			STREAM_PACKET_TYPE_PRIVATE_DH,
			STREAM_PACKET_TYPE_H264,
			STREAM_PACKET_TYPE_AAC,
		}StreamPacketType;

		typedef enum class tagStreamFrameType_t : int
		{
			STREAM_FRAME_TYPE_NONE = 0,
			STREAM_FRAME_TYPE_HEADER = 1,
			STREAM_FRAME_TYPE_STREAM_DATA = 2,
			STREAM_FRAME_TYPE_STREAM_URL = 3,
			STREAM_FRAME_TYPE_I = 4,
			STREAM_FRAME_TYPE_P = 5,
			STREAM_FRAME_TYPE_B = 6,
			STREAM_FRAME_TYPE_AAC = 7,
			STREAM_FRAME_TYPE_JPEG = 8,
			STREAM_FRAME_TYPE_YV12 = 9,
			STREAM_FRAME_TYPE_YUV420P = 10,
			STREAM_FRAME_TYPE_BGR = 11
		}StreamFrameType;

		typedef enum tagStreamHeadSize_t : int
		{
			eFixedHeadFlag = 0x050301FF,
			eFixedHeadSize = 36
		}StreamHeadSize;

		class StreamPacket : public DataPacket
		{
		public:
			StreamPacket(
				const StreamPacketType pktType = StreamPacketType::STREAM_PACKET_TYPE_NONE,
				const StreamFrameType frameType = StreamFrameType::STREAM_FRAME_TYPE_NONE);
			virtual ~StreamPacket(void);

		public:
			//获取流包类型
			//@Return : 流包类型
			inline const StreamPacketType getStreamPacketType(void) const
			{
				return streamPacketType;
			}

			//获取帧类型
			//@Return : 帧类型
			inline const StreamFrameType getStreamFrameType(void) const
			{
				return streamFrameType;
			}

			int buildStreamData(
				const unsigned char* data = nullptr, const int bytes = 0);

			int setStreamData(
				const unsigned char* data = nullptr, const int bytes = 0);

			inline const unsigned char* getStreamData(void) const
			{
				return streamPacketData;
			}
			inline const int getStreamDataBytes(void) const
			{
				return streamPacketDataBytes;
			}

			//设置/获取图片的宽度和高度
			inline void setImageWithAndHeight(const int w = 0, const int h = 0)
			{
				imageWidth = w;
				imageHeight = h;
			}
			inline void getImageWidthAndHeight(int& w, int& h)
			{
				w = imageWidth;
				h = imageHeight;
			}

			//设置/获取报警区域
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
 			const StreamPacketType streamPacketType;
			const StreamFrameType streamFrameType;
			unsigned char* streamPacketData;
			int streamPacketDataBytes;
			int alarmX;
			int alarmY;
			int alarmW;
			int alarmH;
			int imageWidth;
			int imageHeight;
		};//class StreamPacket
	}//namespace packet
}//namespace base

#endif//BASE_PACKET_STREAM_PACKET_H
