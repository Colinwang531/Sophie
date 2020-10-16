//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					YV12格式转换类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
//

#ifndef BASE_STREAM_YV12_TO_YUV420P_CONVERTER_H
#define BASE_STREAM_YV12_TO_YUV420P_CONVERTER_H

#include "Converter/AbstractImageConverter.h"

namespace base
{
	namespace stream
	{
		class YV12ToYUV420PConverter : public AbstractImageConverter
		{
		public:
			YV12ToYUV420PConverter(AbstractFilter& filter);
			virtual ~YV12ToYUV420PConverter(void);

		public:
			int inputData(StreamPacketPtr pkt) override;

		private:
			//创建和销毁YUV420P数据缓存
			//@w : 宽度
			//@h : 高度
			//@Return : 错误码
			int createNewBuffer(const int w = 1920, const int h = 1080);
			int destroyBuffer(void);

		private:
			unsigned char* yuv420pData;
			int yuv420pDataBytes;
		};//class YV12ToYUV420PConverter
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_YV12_TO_YUV420P_CONVERTER_H
