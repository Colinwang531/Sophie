#include "Error.h"
#include "AI/AbstractAlgorithm.h"

namespace base
{
	namespace ai
	{
		AbstractAlgorithm::AbstractAlgorithm(
			const std::string cid,
			const AlgorithmType type /* = AlgorithmType::ALGORITHM_TYPE_NONE */,
			const int gpu /* = 0 */)
			: algorithmType{ type }, gpuID{ gpu }, detectThreshold{ 0.0f }, trackThreshold{ 0.0f }, cameraID{ cid }
		{}
		AbstractAlgorithm::~AbstractAlgorithm(){}

		int AbstractAlgorithm::setFaceCompareSimilar(const float similar /* = 0.52f */)
		{
			return eNotSupport;
		}

		int AbstractAlgorithm::getFaceCompareSimilar(float& similar) const
		{
			return eNotSupport;
		}

		int AbstractAlgorithm::setExtendDetectThreshold(const float threshold /* = 0.0f */)
		{
			return eNotSupport;
		}

		int AbstractAlgorithm::getExtendDetectThreshold(float& threshold) const
		{
			return eNotSupport;
		}
	}//namespace ai
}//namespace base
