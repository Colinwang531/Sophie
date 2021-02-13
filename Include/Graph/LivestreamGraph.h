//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					实时音视频流图类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
//

#ifndef FRAMEWORK_MULTIMEDIA_LIVE_STREAM_GRAPH_H
#define FRAMEWORK_MULTIMEDIA_LIVE_STREAM_GRAPH_H

#include "Graph.h"

namespace framework
{
	namespace multimedia
	{
		class LivestreamGraph : public Graph
		{
		public:
			LivestreamGraph(void);
			virtual ~LivestreamGraph(void);

		protected:
			int addFilter(const std::string name) override;
			int removeFilter(const std::string name) override;
		};//class LivestreamGraph
	}//namespace multimedia
}//namespace framework

#endif//FRAMEWORK_MULTIMEDIA_LIVE_STREAM_GRAPH_H
