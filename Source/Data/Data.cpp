#include <string.h>
#include "boost/checked_delete.hpp"
#include "Error.h"
#include "Data/Data.h"

namespace framework
{
	namespace data
	{
		Data::Data(
			const DataFactory factory /* = DataFactory::DATA_FACTORY_NONE */,
			const DataType type /* = DataType::DATA_TYPE_NONE */)
			: buffer{ nullptr }, bufsize{ 0 }, dataFactory{ factory }, dataType{ type }
		{}
		Data::~Data()
		{
			boost::checked_array_delete(buffer);
		}

		int Data::setData(
			const void* data /* = nullptr */,
			const unsigned long long bytes /* = 0 */)
		{
			int e{ data && 0 < bytes ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				boost::checked_array_delete(buffer);
				unsigned char* buf{ new(std::nothrow) unsigned char[bytes + 1] };

				if (buf)
				{
					buf[bytes] = 0;
#ifdef WINDOWS
					memcpy_s(buf, bytes, data, bytes);
#else
					memcpy(buf, data, bytes);
#endif//WINDOWS

					buffer = buf;
					bufsize = bytes;
				}
				else
				{
					e = eBadNewMemory;
				}
			}

			return e;
		}
	}//namespace data
}//namespace framework
