//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					ʵʱ����Ƶ��CVͼ��
//
//		History:						Author									Date										Description
//										������									2020-09-23									����
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
