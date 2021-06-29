#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "uuid.h"

namespace framework
{
	namespace libcommon
	{
		Uuid::Uuid()
		{}
		Uuid::~Uuid()
		{}

		const std::string Uuid::generateRandomUuid()
		{
			return boost::uuids::to_string(boost::uuids::random_generator()());
		}
	}//namespace libcommon
}//namespace framework
