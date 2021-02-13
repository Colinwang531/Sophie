//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-05
//		Description :					��ȫñ���ʵʱ��ͼ��
//
//		History:						Author									Date										Description
//										������									2020-10-05									����
//

#ifndef HELMET_DETECT_LIVE_STREAM_GRAPH_H
#define HELMET_DETECT_LIVE_STREAM_GRAPH_H

#include "Graph/AI/LivestreamAIGraph.h"
using LivestreamAIGraph = base::stream::LivestreamAIGraph;
using AbstractFilterPtr = base::stream::AbstractFilterPtr;

class HelmetDetectLivestreamGraph : public LivestreamAIGraph
{
public:
	HelmetDetectLivestreamGraph(void);
	virtual ~HelmetDetectLivestreamGraph(void);

protected:
	AbstractFilterPtr createNewAIFilter(void) override;
};//class HelmetDetectLivestreamGraph

#endif//HELMET_DETECT_LIVE_STREAM_GRAPH_H
