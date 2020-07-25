//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-18
//		Description :					安全帽检测类
//
//		History:						Author									Date										Description
//										王科威									2020-07-18									创建
//

#ifndef BASE_AI_HELMET_DETECT_H
#define BASE_AI_HELMET_DETECT_H

#include "AI/AbstractAlgorithm.h"

namespace base
{
	namespace ai
	{
		class HelmetDetect : public AbstractAlgorithm
		{
		public:
			HelmetDetect(
				const std::string cid, const int gpu = 0);
			virtual ~HelmetDetect(void);
		};//class HelmetDetect
	}//namespace ai
}//namespace base

#endif//BASE_AI_HELMET_DETECT_H
