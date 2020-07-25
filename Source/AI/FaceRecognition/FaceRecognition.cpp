#include "Error.h"
#include "AI/FaceRecognition/FaceRecognition.h"

namespace base
{
	namespace ai
	{
		FaceRecognition::FaceRecognition(
			const std::string cid,
			const AlgorithmType type /* = AlgorithmType::ALGORITHM_TYPE_NONE */,
			const int gpu /* = 0 */)
			: AbstractAlgorithm(cid, type, gpu), similarThreshold{ 0.52f }
		{}
		FaceRecognition::~FaceRecognition() {}

		int FaceRecognition::setFaceCompareSimilar(const float threshold /* = 0.0f */)
		{
			similarThreshold = threshold;
			return eSuccess;
		}

		int FaceRecognition::getFaceCompareSimilar(float& threshold) const
		{
			threshold = similarThreshold;
			return eSuccess;
		}
	}//namespace ai
}//namespace base