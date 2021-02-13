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

#ifndef FRAMEWORK_MULTIMEDIA_IMAGE_X2Y_CONVERTER_H
#define FRAMEWORK_MULTIMEDIA_IMAGE_X2Y_CONVERTER_H

#include "boost/shared_ptr.hpp"
#include "Data/Data.h"
using Data = framework::data::Data;
using DataPtr = boost::shared_ptr<Data>;

namespace framework
{
	namespace multimedia
	{
		class ImageX2YConverter
		{
		public:
			ImageX2YConverter(void);
			~ImageX2YConverter(void);

		public:
			int inputData(DataPtr data);

		private:
			void* iframe;
			void* oframe;
			void* ibuffer;
			void* obuffer;
			int ibufsize;
			int obufsize;
			void* ctx;
		};//class ImageX2YConverter
	}//namespace multimedia
}//namespace framework

#endif//FRAMEWORK_MULTIMEDIA_IMAGE_X2Y_CONVERTER_H
