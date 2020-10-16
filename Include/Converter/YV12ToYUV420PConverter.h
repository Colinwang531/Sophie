//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					YV12��ʽת����
//
//		History:						Author									Date										Description
//										������									2020-09-23									����
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
			//����������YUV420P���ݻ���
			//@w : ���
			//@h : �߶�
			//@Return : ������
			int createNewBuffer(const int w = 1920, const int h = 1080);
			int destroyBuffer(void);

		private:
			unsigned char* yuv420pData;
			int yuv420pDataBytes;
		};//class YV12ToYUV420PConverter
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_YV12_TO_YUV420P_CONVERTER_H
