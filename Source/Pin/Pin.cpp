#include "Error.h"
#include "Filter/Filter.h"
#include "Pin/Pin.h"

namespace framework
{
	namespace multimedia
	{
		Pin::Pin(Filter& filter, const PinType type /* = PinType::PIN_TYPE_INPUT */)
			: pinType{ type }, parentFilter{ filter }
		{}
		Pin::~Pin()
		{}

		int Pin::joinPin(PinPtr input)
		{
			int e{ input ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				if (PinType::PIN_TYPE_OUTPUT == pinType)
				{
					inputPin = input;
				}
				else
				{
					e = eNotSupport;
				}
			}

			return e;
		}

		int Pin::inputData(DataPtr data)
		{
			int e{ data ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				if (PinType::PIN_TYPE_INPUT == getPinType())
				{
					e = parentFilter.inputData(data);
				} 
				else if (PinType::PIN_TYPE_OUTPUT == getPinType() && inputPin)
				{
					e = inputPin->inputData(data);
				}
			}

			return e;
		}
	}//namespace multimedia
}//namespace framework
