//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					AI算法Filter类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
//

#ifndef BASE_STREAM_AI_FILTER_H
#define BASE_STREAM_AI_FILTER_H

#include "AI/AbstractAlgorithm.h"
using AbstractAlgorithm = base::ai::AbstractAlgorithm;
using AbstractAlgorithmPtr = boost::shared_ptr<AbstractAlgorithm>;
#include "Filter/AbstractFilter.h"

namespace base
{
	namespace stream
	{
		BOOST_STATIC_CONSTANT(std::string, AIFilterInputPinName = "AIFilterInputPin");
		BOOST_STATIC_CONSTANT(std::string, AIFilterOutputPinName = "AIFilterOutputPin");

		class AIFilter : public AbstractFilter
		{
		public:
			AIFilter(void);
			virtual ~AIFilter(void);

		protected:
			int inputData(StreamPacketPtr pkt) override;
			int createNewPin(void) override;
			int afterProcessInputDataNotification(StreamPacketPtr pkt) override;

		protected:
			AbstractAlgorithmPtr abstractAlgorithmPtr;
		};//class AIFilter
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_AI_FILTER_H
