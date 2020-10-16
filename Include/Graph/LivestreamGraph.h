//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					实时流图类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
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
		//实时流图提供YV12到YUV420p数据转换过滤器
		//如果输入数据就是YUV420p则过滤器不作任何处理
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
