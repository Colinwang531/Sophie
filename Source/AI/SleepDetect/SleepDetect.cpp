#include "AI/SleepDetect/SleepDetect.h"

namespace base
{
	namespace ai
	{
		SleepDetect::SleepDetect(
			const std::string cid, const int gpu /* = 0 */)
			: AbstractAlgorithm(cid, AlgorithmType::ALGORITHM_TYPE_SLEEP, gpu)
		{}
		SleepDetect::~SleepDetect() {}
	}//namespace ai
}//namespace base