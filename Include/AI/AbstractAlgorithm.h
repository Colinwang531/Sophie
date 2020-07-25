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
				const std::string cid,
				const AlgorithmType type = AlgorithmType::ALGORITHM_TYPE_NONE,
				const int gpu = 0);
			virtual ~AbstractAlgorithm(void);

		public:
			//获取算法类型
			//@Return : 算法类型
			inline const AlgorithmType getAlgorithmType(void) const
			{
				return algorithmType;
			}

			//获取GPUID标识
			//@Return : GPUID标识
			inline const int getGPUID(void) const
			{
				return gpuID;
			}

			//读/写算法检测阈值
			//@threshold : 检测阈值
			//@Return : 检测阈值
			inline void setDetectThreshold(const float threshold = 0.0f)
			{
				detectThreshold = threshold;
			}
			inline const float getDetectThreshold(void) const
			{
				return detectThreshold;
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

			//读/写人脸检测相似度阈值
			//@similar : 相似度阈值
			//@Return : 错误码
			//@Comment : 该方法仅人脸算法实现
			virtual int setFaceCompareSimilar(const float similar = 0.52f);
			virtual int getFaceCompareSimilar(float& similar) const;

			//读/写算法扩展检测阈值
			//@threshold : 扩展检测阈值
			//@Return : 错误码
			//@Comment : 该方法仅打电话算法实现
			virtual int setExtendDetectThreshold(const float threshold = 0.0f);
			virtual int getExtendDetectThreshold(float& threshold) const;

		private:
			const AlgorithmType algorithmType;
			const int gpuID;
			float detectThreshold;
			float trackThreshold;
			const std::string cameraID;
		};//class AbstractAlgorithm
	}//namespace ai
}//namespace base

#endif//BASE_AI_ABSTRACT_ALGORITHM_H
