//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					海康SDK解码器类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
//

#ifndef BASE_STREAM_HIKVISION_SDK_DECODER_H
#define BASE_STREAM_HIKVISION_SDK_DECODER_H

#include "PlayCtrl_V7.3.7.30_Build20190117_win64/WindowsPlayM4.h"
#include "DataStruct/FIFOQueue.h"
#include "Decoder/AbstractDecoder.h"

namespace base
{
	namespace stream
	{
		class HikvisionSDKDecoder : public AbstractDecoder
		{
		public:
			HikvisionSDKDecoder(AbstractFilter& filter);
			virtual ~HikvisionSDKDecoder(void);

		protected:
			int inputData(StreamPacketPtr pkt) override;
			void processingDecodeData(void) override;

		private:
			static void CALLBACK afterDecodeOneFrameCallback(
				long id, char* data, long bytes, FRAME_INFO* frame, void* ctx, void*);
			void processInputDataNotification(StreamPacketPtr pkt);

		private:
			LONG decoderID;
			long long decodeFrameNumber;
			FIFOQueue<StreamPacketPtr> streamPacketGroup;
		};//class HikvisionSDKDecoder
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_HIKVISION_SDK_DECODER_H
