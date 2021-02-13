//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					JPEG图片编码过滤器类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
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
