//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					实时音视频流CV图类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
//

#ifndef FRAMEWORK_MULTIMEDIA_LIVE_STREAM_CV_GRAPH_H
#define FRAMEWORK_MULTIMEDIA_LIVE_STREAM_CV_GRAPH_H

#include "Graph/LivestreamGraph.h"

namespace framework
{
	namespace multimedia
	{
		class LivestreamCVGraph : public LivestreamGraph
		{
		public:
			LivestreamCVGraph(void);
			virtual ~LivestreamCVGraph(void);

		protected:
			int addFilter(const std::string name) override;
			int runGraph(void) override;
		};//class LivestreamCVGraph
	}//namespace multimedia
}//namespace framework

#endif//FRAMEWORK_MULTIMEDIA_LIVE_STREAM_CV_GRAPH_H
