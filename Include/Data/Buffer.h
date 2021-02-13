//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-12-14
//		Description :					数据缓存类
//
//		History:						Author									Date										Description
//										王科威									2020-12-14									创建
//

#ifndef FRAMEWORK_DATA_BUFFER_H
#define FRAMEWORK_DATA_BUFFER_H

#include "boost/shared_ptr.hpp"
#include "Data/Data.h"
using Data = framework::data::Data;
using DataPtr = boost::shared_ptr<Data>;

namespace framework
{
	namespace data
	{
		BOOST_STATIC_CONSTANT(int, FixedHeadFlag = 0xff050301);
		BOOST_STATIC_CONSTANT(int, FixedHeadSize = 36);

		class Buffer
		{
		public:
			Buffer(const int bytes = 3 * 1024 * 1024);
			virtual ~Buffer(void);

		public:
			//输入数据
			//@data : 数据
			//@Return : 错误码
			int inputData(DataPtr data);

		protected:
			//解析一帧数据
			virtual void afterParseOneFrameData(DataPtr data) = 0;

		private:
			//封装一帧数据
			int packOneFrameData(void);

		private:
			void* buffer;
			int bufsize;
			DataFactory factory;
			DataType type;
		};//class Buffer
	}//namespace data
}//namespace framework

#endif//FRAMEWORK_DATA_BUFFER_H
