#include "Error.h"
#include "Pin/Pin.h"
#include "Filter/Decoder/StreamDecoderFilter.h"

namespace framework
{
	namespace multimedia
	{
		StreamDecoderFilter::StreamDecoderFilter() 
			: Filter(FilterType::FILTER_TYPE_MEDIUM), Decoder()
		{
			Filter::addPin(InputPinName);
			Filter::addPin(OutputPinName);
		}
		StreamDecoderFilter::~StreamDecoderFilter()
		{}

		int StreamDecoderFilter::inputData(DataPtr data)
		{
			int e{ data ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				e = Decoder::inputData(data);
			}

			return e;
		}

		void StreamDecoderFilter::afterDataDecodeNotification(DataPtr data)
		{
			if (data)
			{
				Filter::inputData(data);
			}
		}
	}//namespace multimedia
}//namespace framework
