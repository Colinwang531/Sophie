#include <string>
#include <sstream>
#include "boost/archive/iterators/base64_from_binary.hpp"
#include "boost/archive/iterators/binary_from_base64.hpp"
#include "boost/archive/iterators/transform_width.hpp"
#include "Error.h"
#include "Data/Base64/Base64.h"

namespace framework
{
	namespace data
	{
		Base64::Base64()
			: Data(DataFactory::DATA_FACTORY_PRIVATE, DataType::DATA_TYPE_BASE64)
		{}
		Base64::~Base64()
		{}

		int Base64::encodeBase64(
			const char* data /* = nullptr */, 
			const int bytes /* = 0 */)
		{
			int e{ data && 0 < bytes ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				typedef boost::archive::iterators::base64_from_binary<
					boost::archive::iterators::transform_width<std::string::const_iterator, 6, 8>> Base64EncodeIterator;
				std::string inputData((const char*)data, bytes);
				std::stringstream outputData;
				copy(Base64EncodeIterator(inputData.begin()), Base64EncodeIterator(inputData.end()), std::ostream_iterator<char>(outputData));
				size_t equal_count = (3 - inputData.length() % 3) % 3;
				for (size_t i = 0; i < equal_count; i++)
				{
					outputData.put('=');
				}
				//去掉结尾数据乱码
				outputData.put('\0');

				const std::string s{ outputData.str() };
				e = Data::setData(s.c_str(), s.length());
			}

			return e;
		}
	}//namespace packet
}//namespace base
