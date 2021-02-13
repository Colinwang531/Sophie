#include "Error.h"
#include "Pin/Pin.h"
#include "Filter/Converter/ImageConverterFilter.h"

namespace framework
{
	namespace multimedia
	{
		ImageConverterFilter::ImageConverterFilter() 
			: Filter(FilterType::FILTER_TYPE_MEDIUM), Converter()
		{
			Filter::addPin(InputPinName);
			Filter::addPin(OutputPinName);
		}
		ImageConverterFilter::~ImageConverterFilter()
		{}

		int ImageConverterFilter::inputData(DataPtr data)
		{
			int e{ data ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				e = Converter::inputData(data);

				if (eSuccess == e)
				{
					e = Filter::inputData(data);
				}
			}

			return e;
		}
	}//namespace multimedia
}//namespace framework
