#include "boost/make_shared.hpp"
#include "Error.h"
#include "AI/AbstractAlgorithm.h"
using AbstractAlgorithm = base::ai::AbstractAlgorithm;
#include "Pin/AbstractPin.h"
#include "Filter/Converter/ImageConverterFilter.h"
#include "Filter/AI/CVHelmetDetectFilter.h"
#include "Filter/AI/CVPhoneDetectFilter.h"
#include "Filter/Encoder/JPEGEncoderFilter.h"
#include "Graph/AI/LivestreamAIGraph.h"

namespace base
{
	namespace stream
	{
		LivestreamAIGraph::LivestreamAIGraph()
			: LivestreamGraph()
		{}

		LivestreamAIGraph::~LivestreamAIGraph()
		{}

		int LivestreamAIGraph::createNewFilter(void* param /* = nullptr */)
		{
			int e{ LivestreamGraph::createNewFilter() };

			AbstractFilterPtr yuv420pConvertBGR24Filter{ boost::make_shared<ImageConverterFilter>() },
				bgr24ConvertYUV420pFilter{ boost::make_shared<ImageConverterFilter>() },
				algorithmFilter{ createNewAIFilter() },
				imageEncoderFilter{ boost::make_shared<JPEGEncoderFilter>() };

			if (yuv420pConvertBGR24Filter && eSuccess == yuv420pConvertBGR24Filter->createNewFilter() &&
				bgr24ConvertYUV420pFilter && eSuccess == bgr24ConvertYUV420pFilter->createNewFilter() &&
				algorithmFilter && eSuccess == algorithmFilter->createNewFilter(param) &&
				imageEncoderFilter && eSuccess == imageEncoderFilter->createNewFilter())
			{
				AbstractGraph::addFilterByName(YUV420pConvertBGR24FilterName, yuv420pConvertBGR24Filter);
				AbstractGraph::addFilterByName(BGR24ConvertYUV420pFilterName, bgr24ConvertYUV420pFilter);
				AbstractGraph::addFilterByName(CVAlgorithmFilterName, algorithmFilter);
				AbstractGraph::addFilterByName(JPEGImageEncoderFilterName, imageEncoderFilter);
				e = eSuccess;
			}

			return e;
		}

		int LivestreamAIGraph::buildupGraph()
		{
			int e{ LivestreamGraph::buildupGraph() };

			if (eSuccess == e)
			{
				e = eBadOperate;
				AbstractFilterPtr yv12ConvertYUV420pFilter{ getFilterByName(YV12PConvertYUV420PFilterName) },
					yuv420pConvertBGR24Filter{ getFilterByName(YUV420pConvertBGR24FilterName) },
					bgr24ConvertYUV420pFilter{ getFilterByName(BGR24ConvertYUV420pFilterName) },
					algorithmFilter{ getFilterByName(CVAlgorithmFilterName) },
					imageEncoderFilter{ getFilterByName(JPEGImageEncoderFilterName) };

				if (yv12ConvertYUV420pFilter && yuv420pConvertBGR24Filter && bgr24ConvertYUV420pFilter && algorithmFilter && imageEncoderFilter)
				{
					AbstractPinPtr yv12ConvertYUV420pOutputPin{ yv12ConvertYUV420pFilter->getPinByName(ImageConverterFilterOutputPinName) },
						yuv420pConvertBGR24InputPin{ yuv420pConvertBGR24Filter->getPinByName(ImageConverterFilterInputPinName) },
						yuv420pConvertBGR24OutputPin{ yuv420pConvertBGR24Filter->getPinByName(ImageConverterFilterOutputPinName) },
						algorithmInputPin{ algorithmFilter->getPinByName(AIFilterInputPinName) },
						algorithmOutputPin{ algorithmFilter->getPinByName(AIFilterOutputPinName) },
						bgr24ConvertYUV420pInputPin{ bgr24ConvertYUV420pFilter->getPinByName(ImageConverterFilterInputPinName) },
						bgr24ConvertYUV420pOutputPin{ bgr24ConvertYUV420pFilter->getPinByName(ImageConverterFilterOutputPinName) },
						imageEncoderInputPin{ imageEncoderFilter->getPinByName(JPEGImageEncoderFilterInputPinName) };

					if (yv12ConvertYUV420pOutputPin && yuv420pConvertBGR24InputPin && yuv420pConvertBGR24OutputPin &&
						algorithmInputPin && algorithmOutputPin && bgr24ConvertYUV420pInputPin && bgr24ConvertYUV420pOutputPin && 
						imageEncoderInputPin)
					{
						yv12ConvertYUV420pOutputPin->connectPin(yuv420pConvertBGR24InputPin);
						yuv420pConvertBGR24OutputPin->connectPin(algorithmInputPin);
						algorithmOutputPin->connectPin(bgr24ConvertYUV420pInputPin);
						bgr24ConvertYUV420pOutputPin->connectPin(imageEncoderInputPin);
						e = eSuccess;
					}
				}
			}

			return e;
		}
	}//namespace stream
}//namespace base
