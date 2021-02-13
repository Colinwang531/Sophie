#include "Device/Device.h"

namespace framework
{
	namespace device
	{
		Device::Device(
			const std::string id, 
			const FactoryType ft /* = FactoryType::FACTORY_TYPE_NONE  */, 
			const DeviceType dt /* = DeviceType::DEVICE_TYPE_NONE */)
			: did{ id }, dfactory{ ft }, dtype{ dt }
		{}
		Device::~Device()
		{}
	}//namespace device
}//namespace framework
