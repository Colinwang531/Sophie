//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					数据缓存过滤器类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
//

#ifndef FRAMEWORK_MULTIMEDIA_DATA_BUFFER_FILTER_H
#define FRAMEWORK_MULTIMEDIA_DATA_BUFFER_FILTER_H

#include "Filter/Filter.h"
#include "Data/Buffer.h"
using Buffer = framework::data::Buffer;

namespace framework
{
	namespace multimedia
	{
		class DataBufferFilter 
			: public Filter, protected Buffer
		{
		public:
			DataBufferFilter(void);
			virtual ~DataBufferFilter(void);

		protected:
			int inputData(DataPtr data) override;
			void afterParseOneFrameData(DataPtr data) override;
		};//class DataBufferFilter
	}//namespace multimedia
}//namespace framework

#endif//FRAMEWORK_MULTIMEDIA_DATA_BUFFER_FILTER_H
