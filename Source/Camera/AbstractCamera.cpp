#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "Camera/AbstractCamera.h"

namespace base
{
	namespace device
	{
		AbstractCamera::AbstractCamera(const int idx /* = -1 */)
			: cameraID{ boost::uuids::to_string(boost::uuids::random_generator()()) }, enableFlag{ true }, cameraIndex{ idx }
		{}
		AbstractCamera::~AbstractCamera(){}
	}//namespace device
}//namespace base
