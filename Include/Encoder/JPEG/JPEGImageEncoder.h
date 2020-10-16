//
//		Copyright :						@2019, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-31
//		Description:					JPEG��������
//
//		History:						Author									Date												Description
//										������									2019-07-31								����
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
			//���������ٱ�����
			//@w : ���
			//@h : �߶�
			//@Return : ������
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
