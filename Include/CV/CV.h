//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-05
//		Description :					CV�㷨��
//
//		History:						Author									Date										Description
//										������									2020-10-05									����
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
			//��ʼ��
			//@param : ����
			//@Return : ������
			int initCV(void* param = nullptr);

			//���ò���
			//@param : ����
			//@Return : ������
			int setParam(void* param = nullptr);

			//��������
			//@data : ����
			//@Return : ������
			int inputData(DataPtr data);

		protected:
			//�㷨������֪ͨ
			//@data : ����
			virtual void afterCVResultOutputNotification(DataPtr data) = 0;

		private:
			//�㷨�������ص�
			//@data : ����
			void afterCVResultOutputCallback(DataPtr data);

		private:
			void* cv;
		};//class CV
	}//namespace ai
}//namespace framework

#endif//FRAMEWORK_ARTIFICIAL_INTELLIGENCE_COMPUTOR_VISION_H
