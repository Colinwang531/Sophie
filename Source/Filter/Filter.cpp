#include "boost/checked_delete.hpp"
#include "boost/make_shared.hpp"
#include "Error.h"
#include "Pin/Pin.h"
#include "Filter/Filter.h"

namespace framework
{
	namespace multimedia
	{
		Filter::Filter(
			const FilterType type /* = FilterType::FILTER_TYPE_MEDIUM */)
			: filterType{ type }
		{}
		Filter::~Filter()
		{
			pins.clear();
		}

		int Filter::addPin(const std::string name)
		{
			int e{ name.empty() ? eInvalidParameter : eSuccess };

			if (eSuccess == e)
			{
				if (!name.compare(InputPinName))
				{
					pins.insert(InputPinName, boost::make_shared<Pin>(*this, PinType::PIN_TYPE_INPUT));
				}
				else if (!name.compare(OutputPinName))
				{
					pins.insert(OutputPinName, boost::make_shared<Pin>(*this, PinType::PIN_TYPE_OUTPUT));
				}
				else
				{
					e = eNotSupport;
				}
			}

			return e;
		}

		int Filter::removePin(const std::string name)
		{
			int e{ name.empty() ? eInvalidParameter : eSuccess };

			if (eSuccess == e)
			{
				pins.remove(name);
			}

			return e;
		}

		int Filter::inputData(DataPtr data)
		{
			int e{ data ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				PinPtr pin{ pins.at(OutputPinName) };
				e = pin ? pin->inputData(data) : eBadOperate;
			}

			return e;
		}

		PinPtr Filter::queryPin(const std::string name)
		{
			return pins.at(name);
		}
	}//namespace multimedia
}//namespace framework
