#include "AI/FightDetect/FightDetect.h"

namespace base
{
	namespace ai
	{
		FightDetect::FightDetect(
			const std::string cid, const int gpu /* = 0 */)
			: AbstractAlgorithm(cid, AlgorithmType::ALGORITHM_TYPE_FIGHT, gpu)
		{}
		FightDetect::~FightDetect() {}
	}//namespace ai
}//namespace base
