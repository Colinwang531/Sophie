#include "Data/AIS/AISData.h"

namespace framework
{
	namespace data
	{
		AISData::AISData() 
			: Data(DataFactory::DATA_FACTORY_AIS, DataType::DATA_TYPE_BYTES)
		{}
		AISData::~AISData()
		{}
	}//namespace data
}//namespace framework
