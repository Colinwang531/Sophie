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
#include "boost/shared_ptr.hpp"
//TODO:��ʹ��StreamPacketʵ������ĿҲҪ����ͷ�ļ�
#include "Packet/Stream/StreamPacket.h"
using StreamPacket = base::packet::StreamPacket;
using StreamPacketPtr = boost::shared_ptr<StreamPacket>;

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

		typedef enum class tagAlgorithmImageParam_t : int
		{
			ALGORITHM_IMAGE_WIDTH = 1920,
			ALGORITHM_IMAGE_HEIGHT = 1080,
			ALGORITHM_IMAGE_CHANNEL = 3
		}AlgorithmImageParam;

		class AbstractAlgorithm : private boost::noncopyable
		{
		public:
			AbstractAlgorithm(
				const AlgorithmType type = AlgorithmType::ALGORITHM_TYPE_NONE);
			AbstractAlgorithm(const AbstractAlgorithm& algo);
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
			inline const int getGpuID(void) const
			{
				return gpuID;
			}

			//��/д�㷨��ID��ʶ
			//@Return : ��ID��ʶ
			inline void setStreamID(const std::string id)
			{
				streamID = id;
			}
			inline const std::string getStreamID(void) const
			{
				return streamID;
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

			//��/д�㷨���ƶ���ֵ
			//@threshold : ���ƶ���ֵ
			//@Return : ���ƶ���ֵ
			inline void setSimilarThreshold(const float threshold = 0.0f)
			{
				similarThreashold = threshold;
			}
			inline const float getSimilarThreshold(void) const
			{
				return similarThreashold;
			}

			//�����������㷨ʵ��
			//@param : �û�����
			//@Return : ������
			virtual int createNewAlgorithm(void* param = nullptr);
			virtual int destroyAlgorithm(void);

			//���������ݰ�
			//@pkt : �����ݰ�
			//@Return : ������
			virtual int inputData(StreamPacketPtr pkt);

			AbstractAlgorithm& operator=(const AbstractAlgorithm& algo);

		protected:
			//�㷨��������
			virtual void processingCalculateData(void);

		private:
			//�㷨���㹤���߳�
			void processCalculateWorkerkThread(void);

		protected:
			bool stopped;
			AlgorithmType algorithmType;
			int gpuID;
			std::string streamID;
			float firstDetectThreshold;
			float secondDetectThreshold;
			float trackThreshold;
			float similarThreashold;
		};//class AbstractAlgorithm
	}//namespace ai
}//namespace base

#endif//BASE_AI_ABSTRACT_ALGORITHM_H
