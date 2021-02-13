//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-05
//		Description :					CV算法类
//
//		History:						Author									Date										Description
//										王科威									2020-10-05									创建
//

#ifndef FRAMEWORK_ARTIFICIAL_INTELLIGENCE_COMPUTOR_VISION_H
#define FRAMEWORK_ARTIFICIAL_INTELLIGENCE_COMPUTOR_VISION_H

#include "boost/shared_ptr.hpp"
#include "Data/Data.h"
using Data = framework::data::Data;
using DataPtr = boost::shared_ptr<Data>;

namespace framework
{
	namespace ai
	{
		class CV
		{
		public:
			CV(void);
			virtual ~CV(void);

		public:
			//初始化
			//@param : 参数
			//@Return : 错误码
			int initCV(void* param = nullptr);

			//设置参数
			//@param : 参数
			//@Return : 错误码
			int setParam(void* param = nullptr);

			//输入数据
			//@data : 数据
			//@Return : 错误码
			int inputData(DataPtr data);

		protected:
			//算法结果输出通知
			//@data : 数据
			virtual void afterCVResultOutputNotification(DataPtr data) = 0;

		private:
			//算法结果输出回调
			//@data : 数据
			void afterCVResultOutputCallback(DataPtr data);

		private:
			void* cv;
		};//class CV
	}//namespace ai
}//namespace framework

#endif//FRAMEWORK_ARTIFICIAL_INTELLIGENCE_COMPUTOR_VISION_H
