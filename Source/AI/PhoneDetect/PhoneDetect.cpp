#include "Error.h"
#include "AI/PhoneDetect/PhoneDetect.h"

namespace base
{
	namespace ai
	{
		PhoneDetect::PhoneDetect(
			const std::string cid, const int gpu /* = 0 */)
			: AbstractAlgorithm(cid, AlgorithmType::ALGORITHM_TYPE_PHONE, gpu)
		{}
		PhoneDetect::~PhoneDetect() {}

		int PhoneDetect::setExtendDetectThreshold(const float threshold /* = 0.0f */)
		{
			extendDetectThreshold = threshold;
			return eSuccess;
		}

		int PhoneDetect::getExtendDetectThreshold(float& threshold) const
		{
			threshold = extendDetectThreshold;
			return eSuccess;
		}
	}//namespace ai
}//namespace base