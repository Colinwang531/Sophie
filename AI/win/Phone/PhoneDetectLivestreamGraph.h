//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-05
//		Description :					打电话检测实时流图类
//
//		History:						Author									Date										Description
//										王科威									2020-10-05									创建
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
