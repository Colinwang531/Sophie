//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-17
//		Description :					AI算法抽象基类
//
//		History:						Author									Date										Description
//										王科威									2020-07-17									创建
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
			//获取算法类型
			//@Return : 算法类型
			inline const AlgorithmType getAlgorithmType(void) const
			{
				return algorithmType;
			}

			//读/写GPU标识
			//@Return : GPU标识
			inline void setGpuID(const int id)
			{
				gpuID = id;
			}
			inline const int getGPUID(void) const
			{
				return gpuID;
			}

			//读/写算法检测阈值
			//@threshold : 检测阈值
			//@Return : 检测阈值
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

			//读/写算法跟踪阈值
			//@threshold : 跟踪阈值
			//@Return : 跟踪阈值
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
