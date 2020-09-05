#include "Error.h"
#include "Device/AbstractDevice.h"

namespace base
{
	namespace device
	{
		AbstractDevice::AbstractDevice(const std::string did)
			: deviceID{ did }
		{}
		AbstractDevice::~AbstractDevice(){}

		int AbstractDevice::startDevice()
		{
			return eSuccess;
		}

		int AbstractDevice::stopDevice()
		{
			return eSuccess;
		}
	}//namespace device
}//namespace base
