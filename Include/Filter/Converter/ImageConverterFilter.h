//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					ͼƬת������
//
//		History:						Author									Date										Description
//										������									2020-09-23									����
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
