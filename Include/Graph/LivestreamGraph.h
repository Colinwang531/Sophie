//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					ʵʱ��ͼ��
//
//		History:						Author									Date										Description
//										������									2020-09-23									����
//

#ifndef BASE_STREAM_LIVE_STREAM_GRAPH_H
#define BASE_STREAM_LIVE_STREAM_GRAPH_H

#include "AbstractGraph.h"

namespace base
{
	namespace stream
	{
		BOOST_STATIC_CONSTANT(std::string, StreamCacheFilterName = "StreamCacheFilter");
		BOOST_STATIC_CONSTANT(std::string, StreamDecoderFilterName = "StreamDecoderFilter");
		//ʵʱ��ͼ�ṩYV12��YUV420p����ת��������
		//����������ݾ���YUV420p������������κδ���
		BOOST_STATIC_CONSTANT(std::string, YV12PConvertYUV420PFilterName = "YV12PConvertYUV420PFilter");

		class LivestreamGraph : public AbstractGraph
		{
		public:
			LivestreamGraph(void);
			virtual ~LivestreamGraph(void);

		protected:
			int createNewFilter(void* = nullptr) override;
			int buildupGraph(void) override;
			int inputData(StreamPacketPtr pkt) override;
		};//class LivestreamGraph
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_LIVE_STREAM_GRAPH_H
