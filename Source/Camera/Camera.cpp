#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "Camera/Camera.h"

namespace framework
{
	namespace device
	{
		Camera::Camera(
			const int idx /* = -1 */)
			: cid{ boost::uuids::to_string(boost::uuids::random_generator()()) }, enable{ true }, index{ idx }
		{}
		Camera::~Camera()
		{}
	}//namespace device
}//namespace framework
