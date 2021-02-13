//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-05
//		Description :					����ʶ��ʵʱ��ͼ��
//
//		History:						Author									Date										Description
//										������									2020-10-05									����
//

#ifndef FACE_RECOGNIZE_DETECT_LIVE_STREAM_GRAPH_H
#define FACE_RECOGNIZE_DETECT_LIVE_STREAM_GRAPH_H

#include "Graph/AI/LivestreamAIGraph.h"
using LivestreamAIGraph = base::stream::LivestreamAIGraph;
using AbstractFilterPtr = base::stream::AbstractFilterPtr;

class FaceRecognizeDetectLivestreamGraph : public LivestreamAIGraph
{
public:
	FaceRecognizeDetectLivestreamGraph(void);
	virtual ~FaceRecognizeDetectLivestreamGraph(void);

protected:
	AbstractFilterPtr createNewAIFilter(void) override;
};//class FaceRecognizeDetectLivestreamGraph

#endif//FACE_RECOGNIZE_DETECT_LIVE_STREAM_GRAPH_H
