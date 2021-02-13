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

#ifndef FRAMEWORK_MULTIMEDIA_YV12_TO_YUV420P_CONVERTER_H
#define FRAMEWORK_MULTIMEDIA_YV12_TO_YUV420P_CONVERTER_H

#include "boost/shared_ptr.hpp"
#include "Data/Data.h"
using Data = framework::data::Data;
using DataPtr = boost::shared_ptr<Data>;

namespace framework
{
	namespace multimedia
	{
		class YV12ToYUV420PConverter
		{
		public:
			YV12ToYUV420PConverter(void);
			~YV12ToYUV420PConverter(void);

		public:
			int inputData(DataPtr data);
		};//class YV12ToYUV420PConverter
	}//namespace multimedia
}//namespace framework

#endif//FRAMEWORK_MULTIMEDIA_YV12_TO_YUV420P_CONVERTER_H
