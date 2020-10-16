#include "boost/make_shared.hpp"
#include "Error.h"
#include "Pin/AbstractPin.h"
#include "Filter/AI/AIFilter.h"

namespace base
{
	namespace stream
	{
		AIFilter::AIFilter() : AbstractFilter()
		{}

		AIFilter::~AIFilter()
		{}

		int AIFilter::inputData(StreamPacketPtr pkt)
		{
			int e{ pkt ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				e = abstractAlgorithmPtr ? abstractAlgorithmPtr->inputData(pkt) : eBadOperate;
			}

			return e;
		}

		int AIFilter::createNewPin()
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
					abstractPinGroup.insert(AIFilterOutputPinName, outputPin);
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
					abstractPinGroup.insert(AIFilterInputPinName, inputPin);
				}
			}

			return 0 < abstractPinGroup.size() ? eSuccess : eBadNewObject;
		}

		int AIFilter::afterProcessInputDataNotification(StreamPacketPtr pkt)
		{
			int e{ pkt ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				AbstractPinPtr outputPin{
					abstractPinGroup.at(AIFilterOutputPinName) };

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
	}//namespace stream
}//namespace base
