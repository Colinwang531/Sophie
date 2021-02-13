#include "boost/make_shared.hpp"
#include "Error.h"
#include "Pin/Pin.h"
#include "Filter/Converter/ImageConverterFilter.h"
#include "Filter/CV/CVComputingFilter.h"
#include "Filter/Encoder/JPEGEncoderFilter.h"
#include "Graph/CV/LivestreamCVGraph.h"

namespace framework
{
	namespace multimedia
	{
		LivestreamCVGraph::LivestreamCVGraph() : LivestreamGraph()
		{
			addFilter(Yuv420pConvertBRG24FilterName);
			addFilter(CVCompuptingFilterName);
			addFilter(JPEGEncoderFilterName);
		}
		LivestreamCVGraph::~LivestreamCVGraph()
		{}

		int LivestreamCVGraph::addFilter(const std::string name)
		{
			int e{ name.empty() ? eInvalidParameter : eSuccess };

			if (eSuccess == e)
			{
				if (!name.compare(Yuv420pConvertBRG24FilterName))
				{
					FilterPtr fp{ boost::make_shared<ImageConverterFilter>() };
					if (fp)
					{
						filters.insert(Yuv420pConvertBRG24FilterName, fp);
					}
				}
				else if (!name.compare(CVCompuptingFilterName))
				{
					FilterPtr fp{ boost::make_shared<CVComputingFilter>() };
					if (fp)
					{
						filters.insert(CVCompuptingFilterName, fp);
					}
				}
				else if (!name.compare(JPEGEncoderFilterName))
				{
					FilterPtr fp{ boost::make_shared<JPEGEncoderFilter>() };
					if (fp)
					{
						filters.insert(JPEGEncoderFilterName, fp);
					}
				}
				else
				{
					e = eNotSupport;
				}
			}

			return e;
		}

		int LivestreamCVGraph::runGraph()
		{
			FilterPtr sdk{ Graph::queryFilter(HKNetSDKFilterName) };
// 			FilterPtr receiver{ Graph::queryFilter(TCPDataReceiverFilterName) };
// 			FilterPtr buffer{ Graph::queryFilter(DataBufferFilterName) };
			FilterPtr decoder{ Graph::queryFilter(StreamDecoderFilterName) };
			FilterPtr yv12yuv420p{ Graph::queryFilter(Yv12ConvertYuv420pFilterName) };
			FilterPtr yuv420pbgr24{ Graph::queryFilter(Yuv420pConvertBRG24FilterName) };
			FilterPtr cv{ Graph::queryFilter(CVCompuptingFilterName) };
			FilterPtr encoder{ Graph::queryFilter(JPEGEncoderFilterName) };
			
			int e{ 
				/*receiver && buffer*/sdk && decoder && yv12yuv420p && yuv420pbgr24 && cv && encoder ? eSuccess : eBadOperate };

			if (eSuccess == e)
			{
// 				receiver->queryPin(OutputPinName)->joinPin(buffer->queryPin(InputPinName));
// 				buffer->queryPin(OutputPinName)->joinPin(decoder->queryPin(InputPinName));
				sdk->queryPin(OutputPinName)->joinPin(decoder->queryPin(InputPinName));
				decoder->queryPin(OutputPinName)->joinPin(yv12yuv420p->queryPin(InputPinName));
				yv12yuv420p->queryPin(OutputPinName)->joinPin(yuv420pbgr24->queryPin(InputPinName));
				yuv420pbgr24->queryPin(OutputPinName)->joinPin(cv->queryPin(InputPinName));
				cv->queryPin(OutputPinName)->joinPin(encoder->queryPin(InputPinName));
			}

			return e;
		}
	}//namespace stream
}//namespace base
