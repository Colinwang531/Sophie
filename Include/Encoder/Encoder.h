//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					编码器类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
//

#ifndef FRAMEWORK_MULTIMEDIA_ENCODER_H
#define FRAMEWORK_MULTIMEDIA_ENCODER_H

#include "boost/shared_ptr.hpp"
#include "Data/Data.h"
using Data = framework::data::Data;
using DataPtr = boost::shared_ptr<Data>;

namespace framework
{
	namespace multimedia
	{
		class Encoder
		{
		public:
			Encoder(void);
			virtual ~Encoder(void);

		public:
			//输入数据
			//@data : 数据
			//@Return : 错误码
			int inputData(DataPtr data);

		protected:
			//解码单帧数据通知
			//@data : 数据
			virtual void afterDataEncodeNotification(DataPtr data) = 0;

		private:
			//编码数据回调
			//@data : 数据
			void afterDataEncodeCallback(DataPtr data);

		private:
			void* encoder;
		};//class Encoder
	}//namespace multimedia
}//namespace framework

#endif//FRAMEWORK_MULTIMEDIA_ENCODER_H
