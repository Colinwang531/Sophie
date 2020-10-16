#include "boost/make_shared.hpp"
#include "Error.h"
#include "Encoder/JPEG/JPEGImageEncoder.h"
#include "Pin/AbstractPin.h"
#include "Filter/Encoder/JPEGEncoderFilter.h"

namespace base
{
	namespace stream
	{
		JPEGEncoderFilter::JPEGEncoderFilter()
			: AbstractFilter(FilterType::FILTER_TYPE_TARGET)
		{}

		JPEGEncoderFilter::~JPEGEncoderFilter()
		{}

		int JPEGEncoderFilter::inputData(StreamPacketPtr pkt)
		{
			int e{ pkt ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				if (!abstractEncoderPtr)
				{
					createNewEncoder();
				}

				e = abstractEncoderPtr ? abstractEncoderPtr->inputData(pkt) : eBadOperate;
			}

			return e;
		}

		int JPEGEncoderFilter::createNewPin()
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
					abstractPinGroup.insert(JPEGImageEncoderFilterOutputPinName, outputPin);
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
					abstractPinGroup.insert(JPEGImageEncoderFilterInputPinName, inputPin);
				}
			}

			return 0 < abstractPinGroup.size() ? eSuccess : eBadNewObject;
		}

		int JPEGEncoderFilter::afterProcessInputDataNotification(StreamPacketPtr pkt)
		{
			int e{ pkt ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				AbstractPinPtr outputPin{
					abstractPinGroup.at(JPEGImageEncoderFilterOutputPinName) };

				if (outputPin)
				{
					e = outputPin->inputData(pkt);
				}
				else if (streamDataCallback)
				{
					streamDataCallback(pkt);
				}
				else
				{
					e = eObjectNotExist;
				}
			}

			return e;
		}

		int JPEGEncoderFilter::createNewEncoder()
		{
			abstractEncoderPtr = boost::make_shared<JPEGImageEncoder>(*this);
			return abstractEncoderPtr ? eSuccess : eBadNewObject;
		}
	}//namespace stream
}//namespace base
