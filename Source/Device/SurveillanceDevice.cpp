#include "Device/SurveillanceDevice.h"

namespace base
{
	namespace device
	{
		SurveillanceDevice::SurveillanceDevice(
			const std::string id, 
			const DeviceFactory factory /* = DeviceFactory::DEVICE_FACTORY_NONE */, 
			const DeviceType type /* = DeviceType::DEVICE_TYPE_NONE */)
			: AbstractDevice(id, factory, type)
		{}
		SurveillanceDevice::~SurveillanceDevice(){}
	}//namespace device
}//namespace base
