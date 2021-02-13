//
//		Copyright :						@2019, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-31
//		Description:					JPEG编码器类
//
//		History:						Author									Date												Description
//										王科威									2019-07-31								创建
//

#ifndef FRAMEWORK_MULTIMEDIA_JPEG_IMAGE_ENCODER_H
#define FRAMEWORK_MULTIMEDIA_JPEG_IMAGE_ENCODER_H

#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include "Data/Data.h"
using Data = framework::data::Data;
using DataPtr = boost::shared_ptr<Data>;

namespace framework
{
	namespace multimedia
	{
		typedef boost::function<void(DataPtr)> DataNotificationCallback;

		class JPEGImageEncoder
		{
		public:
			JPEGImageEncoder(
				DataNotificationCallback callback = nullptr);
			~JPEGImageEncoder(void);

		public:
			int inputData(DataPtr data);

		private:
			void* codec;
			void* ctx;
			void* iframe;
			DataNotificationCallback dataNotificationCallback;
		};//class JPEGImageEncoder
	}//namespace multimedia
}//namespace framework

#endif//FRAMEWORK_MULTIMEDIA_JPEG_IMAGE_ENCODER_H
