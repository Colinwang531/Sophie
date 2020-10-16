//
//		Copyright :						@2019, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-31
//		Description:					JPEG编码器类
//
//		History:						Author									Date												Description
//										王科威									2019-07-31								创建
//

#ifndef BASE_STREAM_JPEG_IMAGE_ENCODER_H
#define BASE_STREAM_JPEG_IMAGE_ENCODER_H

#include <string>
extern "C"
{
#include "libavformat/avformat.h"
}
#include "Encoder/AbstractEncoder.h"

namespace base
{
	namespace stream
	{
		class JPEGImageEncoder : public AbstractEncoder
		{
		public:
			JPEGImageEncoder(AbstractFilter& filter);
			virtual ~JPEGImageEncoder(void);

		public:
			int inputData(StreamPacketPtr pkt) override;

		private:
			//创建和销毁编码器
			//@w : 宽度
			//@h : 高度
			//@Return : 错误码
			int createNewEncoder(const int w = 1920, const int h = 1080);
			int destroyEncoder(void);

			const std::string base64Encode(
				const unsigned char* data = nullptr, 
				const int bytes = 0);

		private:
			AVCodec* avcodec;
			AVCodecContext* avcodecctx;
			AVFrame* srcData;
		};//class YUV420PToJPEGEncoder
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_JPEG_IMAGE_ENCODER_H
