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
#include "boost/shared_ptr.hpp"
//TODO:不使用StreamPacket实例的项目也要引用头文件
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
			inline const int getGpuID(void) const
			{
				return gpuID;
			}

			//读/写算法流ID标识
			//@Return : 流ID标识
			inline void setStreamID(const std::string id)
			{
				streamID = id;
			}
			inline const std::string getStreamID(void) const
			{
				return streamID;
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

			//读/写算法相似度阈值
			//@threshold : 相似度阈值
			//@Return : 相似度阈值
			inline void setSimilarThreshold(const float threshold = 0.0f)
			{
				similarThreashold = threshold;
			}
			inline const float getSimilarThreshold(void) const
			{
				return similarThreashold;
			}

			//创建和销毁算法实例
			//@param : 用户参数
			//@Return : 错误码
			virtual int createNewAlgorithm(void* param = nullptr);
			virtual int destroyAlgorithm(void);

			//输入流数据包
			//@pkt : 流数据包
			//@Return : 错误码
			virtual int inputData(StreamPacketPtr pkt);

			AbstractAlgorithm& operator=(const AbstractAlgorithm& algo);

		protected:
			//算法运算流程
			virtual void processingCalculateData(void);

		private:
			//算法运算工作线程
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
