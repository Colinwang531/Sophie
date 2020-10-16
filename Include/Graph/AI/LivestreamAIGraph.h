//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					实时流AI图类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
//

#ifndef BASE_STREAM_LIVE_STREAM_AI_GRAPH_H
#define BASE_STREAM_LIVE_STREAM_AI_GRAPH_H

#include "Graph/LivestreamGraph.h"

namespace base
{
	namespace stream
	{
		BOOST_STATIC_CONSTANT(std::string, YUV420pConvertBGR24FilterName = "YUV420pConvertBGR24Filter");
		BOOST_STATIC_CONSTANT(std::string, BGR24ConvertYUV420pFilterName = "BGR24ConvertYUV420pFilter");
		BOOST_STATIC_CONSTANT(std::string, CVAlgorithmFilterName = "CVAlgorithmFilter");
		BOOST_STATIC_CONSTANT(std::string, JPEGImageEncoderFilterName = "JPEGImageEncoderFilter");

		class LivestreamAIGraph : public LivestreamGraph
		{
		public:
			LivestreamAIGraph(void);
			virtual ~LivestreamAIGraph(void);

		protected:
			int createNewFilter(void* = nullptr) override;
			int buildupGraph(void) override;
			virtual AbstractFilterPtr createNewAIFilter(void) = 0;
		};//class LivestreamAIGraph
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_LIVE_STREAM_AI_GRAPH_H
