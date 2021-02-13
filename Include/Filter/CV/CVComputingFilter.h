//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					CV�㷨�����������
//
//		History:						Author									Date										Description
//										������									2020-09-23									����
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
			//��ʼ��
			//@param : ����
			//@Return : ������
			int initCV(void* param = nullptr);

			//���ò���
			//@param : ����
			//@Return : ������
			int setParam(void* param = nullptr);

		protected:
			int inputData(DataPtr data) override;
			void afterCVResultOutputNotification(DataPtr data) override;
		};//class CVComputingFilter
	}//namespace multimedia
}//namespace framework

#endif//FRAMEWORK_MULTIMEDIA_CV_COMPUTING_FILTER_H
