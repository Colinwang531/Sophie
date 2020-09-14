#include "AI/AbstractAlgorithm.h"

namespace base
{
	namespace ai
	{
		AbstractAlgorithm::AbstractAlgorithm(
			const AlgorithmType type /* = AlgorithmType::ALGORITHM_TYPE_NONE */)
			: algorithmType{ type }, gpuID{ -1 }, firstDetectThreshold{ 0.0f }, 
			secondDetectThreshold{0.0f}, trackThreshold{ 0.0f }
		{}
		AbstractAlgorithm::~AbstractAlgorithm(){}
	}//namespace ai
}//namespace base
