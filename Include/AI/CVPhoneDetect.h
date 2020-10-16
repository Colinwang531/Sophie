//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-05
//		Description :					打电话检测类
//
//		History:						Author									Date										Description
//										王科威									2020-10-05									创建
//

#ifndef BASE_AI_CV_PHONE_DETECT_H
#define BASE_AI_CV_PHONE_DETECT_H

#include "AI/BasePhoneDll.h"
#include "AI/BaseGpuDetection.h"
#include "DataStruct/FIFOQueue.h"
#include "Filter/AbstractFilter.h"
using AbstractFilter = base::stream::AbstractFilter;
#include "AI/AbstractAlgorithm.h"

namespace base
{
	namespace ai
	{
		class CVPhoneDetect : public AbstractAlgorithm
		{
		public:
			CVPhoneDetect(AbstractFilter& filter);
			virtual ~CVPhoneDetect(void);

		protected:
			int createNewAlgorithm(void* param = nullptr) override;
			int destroyAlgorithm(void) override;
			int inputData(StreamPacketPtr pkt) override;
			void processingCalculateData(void) override;

		private:
			AbstractFilter& abstractFilter;
			CPhoneAlgorithmClass cvPhoneAlgo;
			FeedBackPhone feedbackPhone;
			CGpuDetect cvPhoneGpuDetect;
			FIFOQueue<StreamPacketPtr> imageDataGroup;
			bool enableDetect;
		};//class CVPhoneDetect
	}//namespace ai
}//namespace base

#endif//BASE_AI_CV_PHONE_DETECT_H
