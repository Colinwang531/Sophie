#include "boost/make_shared.hpp"
#include "Error.h"
#include "Filter/SDK/HKNetSDKFilter.h"
// #include "Filter/Network/TCPDataReceiverFilter.h"
// #include "Filter/Cache/DataBufferFilter.h"
#include "Filter/Decoder/StreamDecoderFilter.h"
#include "Filter/Converter/ImageConverterFilter.h"
#include "Graph/LivestreamGraph.h"

namespace framework
{
	namespace multimedia
	{
		LivestreamGraph::LivestreamGraph() : Graph()
		{
			addFilter(HKNetSDKFilterName);
// 			addFilter(TCPDataReceiverFilterName);
// 			addFilter(DataBufferFilterName);
			addFilter(StreamDecoderFilterName);
			addFilter(Yv12ConvertYuv420pFilterName);
		}
		LivestreamGraph::~LivestreamGraph()
		{}

		int LivestreamGraph::addFilter(const std::string name)
		{
			int e{ name.empty() ? eInvalidParameter : eSuccess };

			if (eSuccess == e)
			{
				if (!name.compare(HKNetSDKFilterName))
				{
					FilterPtr fp{ boost::make_shared<HKNetSDKFilter>() };
					if (fp)
					{
						filters.insert(HKNetSDKFilterName, fp);
					}
				}
// 				if (!name.compare(TCPDataReceiverFilterName))
// 				{
// 					FilterPtr fp{ boost::make_shared<TCPDataReceiverFilter>() };
// 					if (fp)
// 					{
// 						filters.insert(TCPDataReceiverFilterName, fp);
// 					}
// 				}
// 				else if (!name.compare(DataBufferFilterName))
// 				{
// 					FilterPtr fp{ boost::make_shared<DataBufferFilter>() };
// 					if (fp)
// 					{
// 						filters.insert(DataBufferFilterName, fp);
// 					}
// 				}
				else if (!name.compare(StreamDecoderFilterName))
				{
					FilterPtr fp{ boost::make_shared<StreamDecoderFilter>() };
					if (fp)
					{
						filters.insert(StreamDecoderFilterName, fp);
					}
				}
				else if (!name.compare(Yv12ConvertYuv420pFilterName))
				{
					FilterPtr fp{ boost::make_shared<ImageConverterFilter>() };
					if (fp)
					{
						filters.insert(Yv12ConvertYuv420pFilterName, fp);
					}
				}
				else
				{
					e = eNotSupport;
				}
			}

			return e;
		}

		int LivestreamGraph::removeFilter(const std::string name)
		{
			int e{ name.empty() ? eInvalidParameter : eSuccess };

			if (eSuccess == e)
			{
				filters.remove(name);
			}

			return e;
		}
	}//namespace stream
}//namespace base
