//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-20
//		Description :					��ܼ����
//
//		History:						Author									Date										Description
//										������									2020-07-20									����
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