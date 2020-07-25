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
				const std::string cid,
				const AlgorithmType type = AlgorithmType::ALGORITHM_TYPE_NONE,
				const int gpu = 0);
			virtual ~AbstractAlgorithm(void);

		public:
			//��ȡ�㷨����
			//@Return : �㷨����
			inline const AlgorithmType getAlgorithmType(void) const
			{
				return algorithmType;
			}

			//��ȡGPUID��ʶ
			//@Return : GPUID��ʶ
			inline const int getGPUID(void) const
			{
				return gpuID;
			}

			//��/д�㷨�����ֵ
			//@threshold : �����ֵ
			//@Return : �����ֵ
			inline void setDetectThreshold(const float threshold = 0.0f)
			{
				detectThreshold = threshold;
			}
			inline const float getDetectThreshold(void) const
			{
				return detectThreshold;
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

			//��/д����������ƶ���ֵ
			//@similar : ���ƶ���ֵ
			//@Return : ������
			//@Comment : �÷����������㷨ʵ��
			virtual int setFaceCompareSimilar(const float similar = 0.52f);
			virtual int getFaceCompareSimilar(float& similar) const;

			//��/д�㷨��չ�����ֵ
			//@threshold : ��չ�����ֵ
			//@Return : ������
			//@Comment : �÷�������绰�㷨ʵ��
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
