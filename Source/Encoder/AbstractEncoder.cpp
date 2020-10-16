#include "Filter/AbstractFilter.h"
#include "Encoder/AbstractEncoder.h"

namespace base
{
	namespace stream
	{
		AbstractEncoder::AbstractEncoder(AbstractFilter& filter)
			: abstractFilter{ filter }
		{}

		AbstractEncoder::~AbstractEncoder()
		{}
	}//namespace stream
}//namespace base