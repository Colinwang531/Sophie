//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					ͼƬ��������
//
//		History:						Author									Date										Description
//										������									2020-09-23									����
//

#ifndef BASE_STREAM_JPEG_ENCODER_FILTER_H
#define BASE_STREAM_JPEG_ENCODER_FILTER_H

#include "boost/shared_ptr.hpp"
#include "Filter/AbstractFilter.h"

namespace base
{
	namespace stream
	{
		BOOST_STATIC_CONSTANT(std::string, JPEGImageEncoderFilterInputPinName = "JPEGImageEncoderFilterInputPin");
		BOOST_STATIC_CONSTANT(std::string, JPEGImageEncoderFilterOutputPinName = "JPEGImageEncoderFilterOutputPin");

		class AbstractEncoder;
		using AbstractEncoderPtr = boost::shared_ptr<AbstractEncoder>;

		class JPEGEncoderFilter : public AbstractFilter
		{
		public:
			JPEGEncoderFilter(void);
			virtual ~JPEGEncoderFilter(void);

		protected:
			int inputData(StreamPacketPtr pkt) override;
			int createNewPin(void) override;
			int afterProcessInputDataNotification(StreamPacketPtr pkt) override;

		private:
			//����������ʵ��
			//@Return : ������
			//@Comment : ���յ���һ���������ݺ��ٴ���ת����ʵ��
			int createNewEncoder(void);

		private:
			AbstractEncoderPtr abstractEncoderPtr;
		};//class JPEGEncoderFilter
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_JPEG_ENCODER_FILTER_H
