//
//		Copyright :						@2019, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description:					图片X->Y转换类
//
//		History:						Author									Date														Description
//										王科威									2020-09-23										创建
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
			//创建和销毁转换数据缓存
			//@src : 输入帧格式
			//@tar : 输出帧格式
			//@w : 宽度
			//@h : 高度
			//@Return : 错误码
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
