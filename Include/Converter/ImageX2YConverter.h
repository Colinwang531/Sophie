//
//		Copyright :						@2019, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description:					ͼƬX->Yת����
//
//		History:						Author									Date														Description
//										������									2020-09-23										����
//

#ifndef BASE_STREAM_IMAGE_X_2_Y_CONVERTER_H
#define BASE_STREAM_IMAGE_X_2_Y_CONVERTER_H

extern "C"
{
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
}
#include "Converter/AbstractImageConverter.h"

namespace base
{
	namespace stream
	{
		class ImageX2YConverter : public AbstractImageConverter
		{
		public:
			ImageX2YConverter(AbstractFilter& filter);
			virtual ~ImageX2YConverter(void);

		public:
			int inputData(StreamPacketPtr pkt) override;

		private:
			//����������ת�����ݻ���
			//@src : ����֡��ʽ
			//@tar : ���֡��ʽ
			//@w : ���
			//@h : �߶�
			//@Return : ������
			int createNewBuffer(
				const AVPixelFormat src = AV_PIX_FMT_NONE,
				const AVPixelFormat tar = AV_PIX_FMT_NONE,
				const int w = 1920, 
				const int h = 1080);
			int destroyBuffer(void);

		private:
			AVFrame* srcAVFrame;
			AVFrame* tarAVFrame;
			unsigned char* srcData;
			unsigned char* tarData;
			struct SwsContext* swsContext;
		};//class ImageX2YConverter
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_IMAGE_X_2_Y_CONVERTER_H
