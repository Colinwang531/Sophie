//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-05
//		Description :					睡觉检测类
//
//		History:						Author									Date										Description
//										王科威									2020-10-05									创建
//

#ifndef BASE_AI_CV_SLEEP_DETECT_H
#define BASE_AI_CV_SLEEP_DETECT_H

#include "AI/BaseSleepDll.h"
#include "DataStruct/FIFOQueue.h"
#include "Filter/AbstractFilter.h"
using AbstractFilter = base::stream::AbstractFilter;
#include "AI/AbstractAlgorithm.h"

namespace base
{
	namespace ai
	{
		class CVSleepDetect : public AbstractAlgorithm
		{
		public:
			CVSleepDetect(AbstractFilter& filter);
			virtual ~CVSleepDetect(void);

		protected:
			int createNewAlgorithm(void* param = nullptr) override;
			int destroyAlgorithm(void) override;
			int inputData(StreamPacketPtr pkt) override;
			void processingCalculateData(void) override;

		private:
			AbstractFilter& abstractFilter;
			CSleepAlgorithmClass cvSleepAlgo;
			FeedBackSleep feedbackSleep;
			FIFOQueue<StreamPacketPtr> imageDataGroup;
			bool enableDetect;
		};//class CVSleepDetect
	}//namespace ai
}//namespace base

#endif//BASE_AI_CV_SLEEP_DETECT_H
