#include "Filter/AbstractFilter.h"
#include "Converter/AbstractImageConverter.h"

namespace base
{
	namespace stream
	{
		AbstractImageConverter::AbstractImageConverter(AbstractFilter& filter) 
			: abstractFilter{ filter }
		{}

		AbstractImageConverter::~AbstractImageConverter()
		{}
	}//namespace stream
}//namespace base
