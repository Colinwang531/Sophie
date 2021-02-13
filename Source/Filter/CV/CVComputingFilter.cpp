#include "Error.h"
#include "Pin/Pin.h"
#include "Filter/CV/CVComputingFilter.h"

namespace framework
{
	namespace multimedia
	{
		CVComputingFilter::CVComputingFilter() 
			: Filter(FilterType::FILTER_TYPE_MEDIUM), CV()
		{
			Filter::addPin(InputPinName);
			Filter::addPin(OutputPinName);
		}
		CVComputingFilter::~CVComputingFilter()
		{}

		int CVComputingFilter::initCV(void* param /* = nullptr */)
		{
			return param ? CV::initCV(param) : eInvalidParameter;
		}

		int CVComputingFilter::setParam(void* param /* = nullptr */)
		{
			return CV::setParam(param);
		}

		int CVComputingFilter::inputData(DataPtr data)
		{
			int e{ data ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				e = CV::inputData(data);
			}

			return e;
		}

		void CVComputingFilter::afterCVResultOutputNotification(DataPtr data)
		{
			if (data)
			{
				Filter::inputData(data);
			}
		}
	}//namespace multimedia
}//namespace framework
