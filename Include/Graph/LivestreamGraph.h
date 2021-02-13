//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					ʵʱ����Ƶ��ͼ��
//
//		History:						Author									Date										Description
//										������									2020-09-23									����
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
