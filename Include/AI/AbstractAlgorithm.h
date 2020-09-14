//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-17
//		Description :					AI�㷨�������
//
//		History:						Author									Date										Description
//										������									2020-07-17									����
//

#ifndef BASE_AI_ABSTRACT_ALGORITHM_H
#define BASE_AI_ABSTRACT_ALGORITHM_H

#include <string>
#include "boost/noncopyable.hpp"

namespace base
{
	namespace ai
	{
		typedef enum class tagAlgorithmType_t : int
		{
			ALGORITHM_TYPE_NONE = 0,
			ALGORITHM_TYPE_HELMET = 1,
			ALGORITHM_TYPE_PHONE = 2,
			ALGORITHM_TYPE_SLEEP = 3,
			ALGORITHM_TYPE_FIGHT = 4,
			ALGORITHM_TYPE_ATTENDANCE_IN = 5,
			ALGORITHM_TYPE_ATTENDANCE_OUT = 6
		}AlgorithmType;

		class AbstractAlgorithm : private boost::noncopyable
		{
		public:
			AbstractAlgorithm(
				const AlgorithmType type = AlgorithmType::ALGORITHM_TYPE_NONE);
			virtual ~AbstractAlgorithm(void);

		public:
			//��ȡ�㷨����
			//@Return : �㷨����
			inline const AlgorithmType getAlgorithmType(void) const
			{
				return algorithmType;
			}

			//��/дGPU��ʶ
			//@Return : GPU��ʶ
			inline void setGpuID(const int id)
			{
				gpuID = id;
			}
			inline const int getGPUID(void) const
			{
				return gpuID;
			}

			//��/д�㷨�����ֵ
			//@threshold : �����ֵ
			//@Return : �����ֵ
			inline void setDetectThreshold(const float first = 0.0f, const float second = 0.0f)
			{
				firstDetectThreshold = first;
				secondDetectThreshold = second;
			}
			inline const float getFirstDetectThreshold(void) const
			{
				return firstDetectThreshold;
			}
			inline const float getSecondDetectThreshold(void) const
			{
				return secondDetectThreshold;
			}

			//��/д�㷨������ֵ
			//@threshold : ������ֵ
			//@Return : ������ֵ
			inline void setTrackThreshold(const float threshold = 0.0f)
			{
				trackThreshold = threshold;
			}
			inline const float getTrackThreshold(void) const
			{
				return trackThreshold;
			}

		private:
			const AlgorithmType algorithmType;
			int gpuID;
			float firstDetectThreshold;
			float secondDetectThreshold;
			float trackThreshold;
		};//class AbstractAlgorithm
	}//namespace ai
}//namespace base

#endif//BASE_AI_ABSTRACT_ALGORITHM_H
