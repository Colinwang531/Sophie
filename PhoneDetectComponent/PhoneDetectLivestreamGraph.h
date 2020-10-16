//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-05
//		Description :					��绰���ʵʱ��ͼ��
//
//		History:						Author									Date										Description
//										������									2020-10-05									����
//

#ifndef PHONE_DETECT_LIVE_STREAM_GRAPH_H
#define PHONE_DETECT_LIVE_STREAM_GRAPH_H

#include "Graph/AI/LivestreamAIGraph.h"
using LivestreamAIGraph = base::stream::LivestreamAIGraph;
using AbstractFilterPtr = base::stream::AbstractFilterPtr;

class PhoneDetectLivestreamGraph : public LivestreamAIGraph
{
public:
	PhoneDetectLivestreamGraph(void);
	virtual ~PhoneDetectLivestreamGraph(void);

protected:
	AbstractFilterPtr createNewAIFilter(void) override;
};//class PhoneDetectLivestreamGraph

#endif//PHONE_DETECT_LIVE_STREAM_GRAPH_H
