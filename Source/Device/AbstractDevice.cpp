#include "Error.h"
#include "Device/AbstractDevice.h"
using WriteLocker = base::locker::WriteLocker;

namespace base
{
	namespace device
	{
		unsigned int AbstractDevice::gDeviceCounter = 0;

		AbstractDevice::AbstractDevice(){}
		AbstractDevice::~AbstractDevice(){}

		int AbstractDevice::createNewDevice()
		{
			WriteLocker w{ wrl };
			++gDeviceCounter;
			return eSuccess;
		}

		int AbstractDevice::destoryDevice()
		{
			WriteLocker w{ wrl };
			--gDeviceCounter;
			return eSuccess;
		}

		int AbstractDevice::getDeviceConfig(void)
		{
			return eNotSupport;
		}
	}//namespace device
}//namespace base
