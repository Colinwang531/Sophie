#include "Error.h"
#include "Filter/AbstractFilter.h"

namespace base
{
	namespace stream
	{
		AbstractFilter::AbstractFilter(
			const FilterType type /* = FilterType::FILTER_TYPE_MEDIUM */) 
			: filterType{type}
		{}

		AbstractFilter::~AbstractFilter()
		{
			destroyFilter();
		}

		int AbstractFilter::createNewFilter(void* param /* = nullptr */)
		{
			return createNewPin();
		}

		int AbstractFilter::destroyFilter()
		{
			abstractPinGroup.clear();
			return eSuccess;
		}

		AbstractPinPtr AbstractFilter::getPinByName(const std::string name)
		{
			return abstractPinGroup.at(name);
		}
	}//namespace stream
}//namespace base
