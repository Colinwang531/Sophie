#include "boost/make_shared.hpp"
#include "Error.h"
#include "Pin/AbstractPin.h"
#include "Filter/Cache/StreamCacheFilter.h"
#include "Filter/Decoder/StreamDecoderFilter.h"
#include "Filter/Converter/ImageConverterFilter.h"
#include "Graph/LivestreamGraph.h"

namespace base
{
	namespace stream
	{
		LivestreamGraph::LivestreamGraph() : AbstractGraph()
		{}

		LivestreamGraph::~LivestreamGraph()
		{}

		int LivestreamGraph::createNewFilter(void* /* = nullptr */)
		{
			int e{ eBadNewObject };

			AbstractFilterPtr cacheFilter{ boost::make_shared<StreamCacheFilter>() },
				decoderFilter{ boost::make_shared<StreamDecoderFilter>() },
				converterFilter{ boost::make_shared<ImageConverterFilter>() };

			if (cacheFilter && eSuccess == cacheFilter->createNewFilter() && 
				decoderFilter && eSuccess == decoderFilter->createNewFilter() &&
				converterFilter && eSuccess == converterFilter->createNewFilter())
			{
				AbstractGraph::addFilterByName(StreamCacheFilterName, cacheFilter);
				AbstractGraph::addFilterByName(StreamDecoderFilterName, decoderFilter);
				AbstractGraph::addFilterByName(YV12PConvertYUV420PFilterName, converterFilter);
				e = eSuccess;
			}

			return e;
		}

		int LivestreamGraph::buildupGraph()
		{
			int e{ 0 < abstractFilterGroup.size() ? eSuccess : eBadOperate };

			if (eSuccess == e)
			{
				e = eBadOperate;
				AbstractFilterPtr streamCacheFilter{ getFilterByName(StreamCacheFilterName) },
					streamDecoderFilter{ getFilterByName(StreamDecoderFilterName) },
					imageConverterFilter{ getFilterByName(YV12PConvertYUV420PFilterName) };

				if (streamCacheFilter && streamDecoderFilter && imageConverterFilter)
				{
					AbstractPinPtr streamCacheOutputPin{ streamCacheFilter->getPinByName(StreamCacheFilterOutputPinName) },
						streamDecoderInputPin{ streamDecoderFilter->getPinByName(StreamDecoderFilterInputPinName) },
						streamDecoderOutputPin{ streamDecoderFilter->getPinByName(StreamDecoderFilterOutputPinName) },
						imageConverterInputPin{ imageConverterFilter->getPinByName(ImageConverterFilterInputPinName) };

					if (streamCacheOutputPin && streamDecoderInputPin && streamDecoderOutputPin && imageConverterInputPin)
					{
						streamCacheOutputPin->connectPin(streamDecoderInputPin);
						streamDecoderOutputPin->connectPin(imageConverterInputPin);
						e = eSuccess;
					}
				}
			}

			return e;
		}

		int LivestreamGraph::inputData(StreamPacketPtr pkt)
		{
			int e{ pkt ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				AbstractFilterPtr sourceFilter{ getFilterByName(StreamCacheFilterName) };
				if (sourceFilter)
				{
					e = sourceFilter->inputData(pkt);
				}
				else
				{
					e = eBadOperate;
				}
			}
			return 0;
		}
	}//namespace stream
}//namespace base
