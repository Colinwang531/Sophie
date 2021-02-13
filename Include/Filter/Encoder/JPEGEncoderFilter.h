//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					JPEGͼƬ�����������
//
//		History:						Author									Date										Description
//										������									2020-09-23									����
//

#ifndef FRAMEWORK_MULTIMEDIA_JPEG_ENCODER_FILTER_H
#define FRAMEWORK_MULTIMEDIA_JPEG_ENCODER_FILTER_H

#include "Encoder/Encoder.h"
#include "Filter/Filter.h"

namespace framework
{
	namespace multimedia
	{
		class JPEGEncoderFilter 
			: public Filter, protected Encoder
		{
		public:
			JPEGEncoderFilter(void);
			virtual ~JPEGEncoderFilter(void);

		protected:
			int inputData(DataPtr data) override;
			void afterDataEncodeNotification(DataPtr data) override;
		};//class JPEGEncoderFilter
	}//namespace multimedia
}//namespace framework

#endif//FRAMEWORK_MULTIMEDIA_JPEG_ENCODER_FILTER_H
