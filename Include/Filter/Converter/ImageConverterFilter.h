//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					图片转换器类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
//

#ifndef FRAMEWORK_MULTIMEDIA_IMAGE_CONVERTER_FILTER_H
#define FRAMEWORK_MULTIMEDIA_IMAGE_CONVERTER_FILTER_H

#include "Converter/Converter.h"
#include "Filter/Filter.h"

namespace framework
{
	namespace multimedia
	{
		class ImageConverterFilter 
			: public Filter, protected Converter
		{
		public:
			ImageConverterFilter(void);
			virtual ~ImageConverterFilter(void);

		protected:
			int inputData(DataPtr data) override;
		};//class ImageConverterFilter
	}//namespace multimedia
}//namespace framework

#endif//FRAMEWORK_MULTIMEDIA_IMAGE_CONVERTER_FILTER_H
