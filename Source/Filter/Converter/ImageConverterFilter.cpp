#include "boost/make_shared.hpp"
#include "Error.h"
#include "Converter/YV12ToYUV420PConverter.h"
#include "Converter/ImageX2YConverter.h"
#include "Pin/AbstractPin.h"
#include "Filter/Converter/ImageConverterFilter.h"

namespace base
{
	namespace stream
	{
		ImageConverterFilter::ImageConverterFilter() : AbstractFilter()
		{}

		ImageConverterFilter::~ImageConverterFilter()
		{}

		int ImageConverterFilter::inputData(StreamPacketPtr pkt)
		{
			int e{ pkt ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				if (!abstractImageConverterPtr)
				{
					createNewConverter(pkt->getStreamFrameType());
				}

				e = abstractImageConverterPtr ? abstractImageConverterPtr->inputData(pkt) : eBadOperate;
			}

			return e;
		}

		int ImageConverterFilter::createNewPin()
		{
			const FilterType ft{ AbstractFilter::getFilterType() };

			if (FilterType::FILTER_TYPE_SOURCE == ft || FilterType::FILTER_TYPE_MEDIUM == ft)
			{
				AbstractPinPtr outputPin{
					boost::make_shared<AbstractPin>(
						boost::enable_shared_from_this<AbstractFilter>::shared_from_this(),
						PinType::PIN_TYPE_OUTPUT) };

				if (outputPin)
				{
					abstractPinGroup.insert(ImageConverterFilterOutputPinName, outputPin);
				}
			}

			if (FilterType::FILTER_TYPE_TARGET == ft || FilterType::FILTER_TYPE_MEDIUM == ft)
			{
				AbstractPinPtr inputPin{
					boost::make_shared<AbstractPin>(
						boost::enable_shared_from_this<AbstractFilter>::shared_from_this(),
						PinType::PIN_TYPE_INPUT) };

				if (inputPin)
				{
					abstractPinGroup.insert(ImageConverterFilterInputPinName, inputPin);
				}
			}

			return 0 < abstractPinGroup.size() ? eSuccess : eBadNewObject;
		}

		int ImageConverterFilter::afterProcessInputDataNotification(StreamPacketPtr pkt)
		{
			int e{ pkt ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				AbstractPinPtr outputPin{
					abstractPinGroup.at(ImageConverterFilterOutputPinName) };

				if (outputPin)
				{
					e = outputPin->inputData(pkt);
				}
				else
				{
					e = eObjectNotExist;
				}
			}

			return e;
		}

		int ImageConverterFilter::createNewConverter(
			const base::packet::StreamFrameType type /* = base::packet::StreamFrameType::STREAM_FRAME_TYPE_NONE */)
		{
			if (base::packet::StreamFrameType::STREAM_FRAME_TYPE_YV12 == type)
			{
				AbstractImageConverterPtr cvt{ 
					boost::make_shared<YV12ToYUV420PConverter>(*this) };

				if (cvt)
				{
					abstractImageConverterPtr.swap(cvt);
				}
			}
			else if (base::packet::StreamFrameType::STREAM_FRAME_TYPE_YUV420P == type ||
				base::packet::StreamFrameType::STREAM_FRAME_TYPE_BGR == type)
			{
				AbstractImageConverterPtr cvt{
					boost::make_shared<ImageX2YConverter>(*this) };

				if (cvt)
				{
					abstractImageConverterPtr.swap(cvt);
				}
			}

			return abstractImageConverterPtr ? eSuccess : eBadNewObject;
		}
	}//namespace stream
}//namespace base
