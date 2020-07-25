//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-20
//		Description :					打架检测类
//
//		History:						Author									Date										Description
//										王科威									2020-07-20									创建
//

#ifndef BASE_AI_FIGHT_DETECT_H
#define BASE_AI_FIGHT_DETECT_H

#include "AI/AbstractAlgorithm.h"

namespace base
{
	namespace ai
	{
		class FightDetect : public AbstractAlgorithm
		{
		public:
			FightDetect(
				const std::string cid, const int gpu = 0);
			virtual ~FightDetect(void);
		};//class FightDetect
	}//namespace ai
}//namespace base

#endif//BASE_AI_FIGHT_DETECT_H