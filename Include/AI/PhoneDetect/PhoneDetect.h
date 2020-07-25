//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-20
//		Description :					打电话检测类
//
//		History:						Author									Date										Description
//										王科威									2020-07-20									创建
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
			//读/写算法扩展检测阈值
			//@threshold : 扩展检测阈值
			//@Return : 错误码
			//@Comment : 该方法仅打电话算法实现
			int setExtendDetectThreshold(const float threshold = 0.0f) override;
			int getExtendDetectThreshold(float& threshold) const override;

		private:
			float extendDetectThreshold;
		};//class PhoneDetect
	}//namespace ai
}//namespace base

#endif//BASE_AI_PHONE_DETECT_H