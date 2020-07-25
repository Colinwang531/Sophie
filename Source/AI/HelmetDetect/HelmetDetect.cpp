#include "AI/HelmetDetect/HelmetDetect.h"

namespace base
{
	namespace ai
	{
		HelmetDetect::HelmetDetect(
			const std::string cid, const int gpu /* = 0 */)
			: AbstractAlgorithm(cid, AlgorithmType::ALGORITHM_TYPE_HELMET, gpu)
		{}
		HelmetDetect::~HelmetDetect(){}
	}//namespace ai
}//namespace base
