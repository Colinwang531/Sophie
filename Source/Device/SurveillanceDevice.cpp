#include "Error.h"
#include "Device/SurveillanceDevice.h"

namespace base
{
	namespace device
	{
		int SurveillanceDevice::gDeviceCounter = 0;

		SurveillanceDevice::SurveillanceDevice(
			const std::string did,
			const SurveillanceDeviceFactory factory /* = SurveillanceDeviceFactory::SURVEILLANCE_DEVICE_FACTORY_NONE */,
			const SurveillanceDeviceType type /* = SurveillanceDeviceType::SURVEILLANCE_DEVICE_TYPE_NONE */)
			: AbstractDevice(did), deviceFactory{ factory }, deviceType{ type }
		{}
		SurveillanceDevice::~SurveillanceDevice(){}

		int SurveillanceDevice::getDeviceCamera(std::vector<AbstractCamera>& cameras)
		{
			return eNotSupport;
		}

		int SurveillanceDevice::loginDevice()
		{
			return ++gDeviceCounter;
		}

		int SurveillanceDevice::logoutDevice()
		{
			return 0 < gDeviceCounter ? --gDeviceCounter : 0;
		}
	}//namespace device
}//namespace base
