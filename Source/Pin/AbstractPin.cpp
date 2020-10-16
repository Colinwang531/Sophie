#include "Error.h"
#include "Filter/AbstractFilter.h"
#include "Pin/AbstractPin.h"

namespace base
{
	namespace stream
	{
		AbstractPin::AbstractPin(
			AbstractFilterPtr filter,
			const PinType type /* = PinType::PIN_TYPE_INPUT */) : pinType{ type }, parentFilter{ filter }
		{}

		AbstractPin::~AbstractPin()
		{}

		int AbstractPin::connectPin(AbstractPinPtr inputPin)
		{
			int e{ PinType::PIN_TYPE_OUTPUT == pinType ? eSuccess : eNotSupport };

			if (eSuccess == e)
			{
				nextInputPin = inputPin;
			}

			return e;
		}

		int AbstractPin::inputData(StreamPacketPtr pkt)
		{
			int e{ pkt ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				if (PinType::PIN_TYPE_OUTPUT == pinType && nextInputPin)
				{
					nextInputPin->inputData(pkt);
				} 
				else if (PinType::PIN_TYPE_INPUT == pinType && parentFilter)
				{
					parentFilter->inputData(pkt);
				}
				else
				{
					e = eBadOperate;
				}
			}

			return e;
		}
	}//namespace stream
}//namespace base
