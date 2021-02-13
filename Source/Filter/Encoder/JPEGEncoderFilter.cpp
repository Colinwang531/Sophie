#include "Error.h"
#include "Pin/Pin.h"
#include "Filter/Encoder/JPEGEncoderFilter.h"

namespace framework
{
	namespace multimedia
	{
		JPEGEncoderFilter::JPEGEncoderFilter()
			: Filter(FilterType::FILTER_TYPE_TARGET), Encoder()
		{
			Filter::addPin(InputPinName);
		}
		JPEGEncoderFilter::~JPEGEncoderFilter()
		{}

		int JPEGEncoderFilter::inputData(DataPtr data)
		{
			int e{ data ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				e = Encoder::inputData(data);
			}

			return e;
		}

		void JPEGEncoderFilter::afterDataEncodeNotification(DataPtr data)
		{
			if (data)
			{
				if (dataNotificationCallback)
				{
					dataNotificationCallback(data);
				}
			}
		}
	}//namespace multimedia
}//namespace framework
