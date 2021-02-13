//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					格式转换类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
//

#ifndef FRAMEWORK_MULTIMEDIA_CONVERTER_H
#define FRAMEWORK_MULTIMEDIA_CONVERTER_H

#include "boost/shared_ptr.hpp"
#include "Data/Data.h"
using Data = framework::data::Data;
using DataPtr = boost::shared_ptr<Data>;

namespace framework
{
	namespace multimedia
	{
		class Converter
		{
		public:
			Converter(void);
			virtual ~Converter(void);

		public:
			//输入流数据包
			//@pkt : 流数据包
			//@Return : 错误码
			int inputData(DataPtr data);

		private:
			void* cvter;
		};//class Converter
	}//namespace multimedia
}//namespace framework

#endif//FRAMEWORK_MULTIMEDIA_CONVERTER_H
