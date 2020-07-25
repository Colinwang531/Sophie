//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-20
//		Description :					��绰�����
//
//		History:						Author									Date										Description
//										������									2020-07-20									����
//

#ifndef BASE_AI_PHONE_DETECT_H
#define BASE_AI_PHONE_DETECT_H

#include "AI/AbstractAlgorithm.h"

namespace base
{
	namespace ai
	{
		class PhoneDetect : public AbstractAlgorithm
		{
		public:
			PhoneDetect(
				const std::string cid, const int gpu = 0);
			virtual ~PhoneDetect(void);

		protected:
			//��/д�㷨��չ�����ֵ
			//@threshold : ��չ�����ֵ
			//@Return : ������
			//@Comment : �÷�������绰�㷨ʵ��
			int setExtendDetectThreshold(const float threshold = 0.0f) override;
			int getExtendDetectThreshold(float& threshold) const override;

		private:
			float extendDetectThreshold;
		};//class PhoneDetect
	}//namespace ai
}//namespace base

#endif//BASE_AI_PHONE_DETECT_H