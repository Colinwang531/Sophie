#include "Device/AbstractDevice.h"

namespace base
{
	namespace device
	{
		AbstractDevice::AbstractDevice(
			const std::string id,
			const DeviceFactory factory /* = DeviceFactory::DEVICE_FACTORY_NONE */,
			const DeviceType type /* = DeviceType::DEVICE_TYPE_NONE */)
			: deviceID{ id }, deviceFactory{ factory }, deviceType{ type }
		{}
		AbstractDevice::~AbstractDevice(){}
	}//namespace device
}//namespace base
