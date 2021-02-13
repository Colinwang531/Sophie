#include "Define.h"
#include "Error.h"
#include "Pin/Pin.h"
#include "Filter/Cache/DataBufferFilter.h"

namespace framework
{
	namespace multimedia
	{
		DataBufferFilter::DataBufferFilter()
			: Filter(FilterType::FILTER_TYPE_MEDIUM), Buffer(gMaxBufferSize)
		{
			Filter::addPin(InputPinName);
			Filter::addPin(OutputPinName);
		}
		DataBufferFilter::~DataBufferFilter()
		{}

		int DataBufferFilter::inputData(DataPtr data)
		{
			return data ? Buffer::inputData(data) : eInvalidParameter;
		}

		void DataBufferFilter::afterParseOneFrameData(DataPtr data)
		{
			if (data)
			{
				Filter::inputData(data);
			}
		}
	}//namespace multimedia
}//namespace framework