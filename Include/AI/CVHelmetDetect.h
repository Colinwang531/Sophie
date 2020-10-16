//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-18
//		Description :					安全帽检测类
//
//		History:						Author									Date										Description
//										王科威									2020-07-18									创建
//

#ifndef BASE_AI_CV_HELMET_DETECT_H
#define BASE_AI_CV_HELMET_DETECT_H

#include "AI/BaseHelmetDll.h"
#include "DataStruct/FIFOQueue.h"
#include "Filter/AbstractFilter.h"
using AbstractFilter = base::stream::AbstractFilter;
#include "AI/AbstractAlgorithm.h"

namespace base
{
	namespace ai
	{
		class CVHelmetDetect : public AbstractAlgorithm
		{
		public:
			CVHelmetDetect(AbstractFilter& filter);
			virtual ~CVHelmetDetect(void);

		protected:
			int createNewAlgorithm(void* param = nullptr) override;
			int destroyAlgorithm(void) override;
			int inputData(StreamPacketPtr pkt) override;
			void processingCalculateData(void) override;

		private:
			AbstractFilter& abstractFilter;
			CHelmetAlgorithmClass cvHelmetAlgo;
			FeedBackHelmet feedbackHelmet;
			FIFOQueue<StreamPacketPtr> imageDataGroup;
			bool enableDetect;
		};//class CVHelmetDetect
	}//namespace ai
}//namespace base

#endif//BASE_AI_CV_HELMET_DETECT_H
