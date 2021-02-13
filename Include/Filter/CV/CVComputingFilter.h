//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					CV算法计算过滤器类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
//

#ifndef FRAMEWORK_MULTIMEDIA_CV_COMPUTING_FILTER_H
#define FRAMEWORK_MULTIMEDIA_CV_COMPUTING_FILTER_H

#include "CV/CV.h"
using CV = framework::ai::CV;
#include "Filter/Filter.h"

namespace framework
{
	namespace multimedia
	{
		class CVComputingFilter 
			: public Filter, protected CV
		{
		public:
			CVComputingFilter(void);
			virtual ~CVComputingFilter(void);

		public:
			//初始化
			//@param : 参数
			//@Return : 错误码
			int initCV(void* param = nullptr);

			//设置参数
			//@param : 参数
			//@Return : 错误码
			int setParam(void* param = nullptr);

		protected:
			int inputData(DataPtr data) override;
			void afterCVResultOutputNotification(DataPtr data) override;
		};//class CVComputingFilter
	}//namespace multimedia
}//namespace framework

#endif//FRAMEWORK_MULTIMEDIA_CV_COMPUTING_FILTER_H
