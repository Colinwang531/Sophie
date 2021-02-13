//
//		Copyright :						@2019, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-31
//		Description:					JPEG��������
//
//		History:						Author									Date												Description
//										������									2019-07-31								����
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
