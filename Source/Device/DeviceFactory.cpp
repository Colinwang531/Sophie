#include "boost/make_shared.hpp"
#include "Define.h"
#include "Error.h"
#include "Device/Hikvision/HikvisionDevice.h"
using HikvisionDevice = framework::device::HikvisionDevice;
using HikvisionDevicePtr = boost::shared_ptr<HikvisionDevice>;
#include "Camera/Hikvision/HikvisionCamera.h"
using HikvisionCamera = framework::device::HikvisionCamera;
#include "Device/DeviceFactory.h"

namespace framework
{
	namespace device
	{
		DeviceFactory::DeviceFactory()
		{}
		DeviceFactory::~DeviceFactory()
		{}

		DevicePtr DeviceFactory::createNewDevice(
			const std::string id, 
			const FactoryType ft /* = FactoryType::FACTORY_TYPE_NONE */, 
			const DeviceType dt /* = DeviceType::DEVICE_TYPE_NONE */)
		{
			DevicePtr dp;

			if (!id.empty())
			{
				if (FactoryType::FACTORY_TYPE_HK == ft)
				{
					dp = boost::make_shared<HikvisionDevice>(id, dt);
				}
			}

			return dp;
		}

		int DeviceFactory::loginDevice(
			DevicePtr device, 
			const std::string username, 
			const std::string userpwd, 
			const std::string ipv4, 
			const unsigned short port /* = 0 */)
		{
			int e{ 
				!username.empty() && !userpwd.empty() && !ipv4.empty() && device ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				const FactoryType factory{ device->getDeviceFactory() };

				if (FactoryType::FACTORY_TYPE_HK == factory)
				{
					HikvisionDevicePtr hkdp{ 
						boost::dynamic_pointer_cast<HikvisionDevice>(device) };
					e = hkdp->loginDevice(username, userpwd, ipv4, port);
				}
				else
				{
					e = eNotSupport;
				}
			}

			return e;
		}

		int DeviceFactory::logoutDevice(DevicePtr device)
		{
			int e{ device ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				const FactoryType factory{ device->getDeviceFactory() };

				if (FactoryType::FACTORY_TYPE_HK == factory)
				{
					HikvisionDevicePtr hkdp{
						boost::dynamic_pointer_cast<HikvisionDevice>(device) };
					e = hkdp->logoutDevice();
				}
				else
				{
					e = eNotSupport;
				}
			}

			return e;
		}

		int DeviceFactory::getCameraInfos(
			DevicePtr device, 
			std::vector<CameraInfos>& cinfos)
		{
			int e{ device ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				const FactoryType factory{ device->getDeviceFactory() };

				if (FactoryType::FACTORY_TYPE_HK == factory)
				{
					HikvisionDevicePtr hkdp{
						boost::dynamic_pointer_cast<HikvisionDevice>(device) };
					for (int i = 0; i != 64; ++i)
					{
						CameraPtr cp{ hkdp->getCamera(i) };
						if (cp)
						{
							CameraInfos cis;
							cis.enable = cp->getEnable();
							cis.type = 0;
							cis.idx = cp->getIndex();
							cis.id = cp->getID();
							cis.ipv4 = cp->getIPv4();
							cinfos.push_back(cis);
						}
					}
				}
				else
				{
					e = eNotSupport;
				}
			}

			return e;
		}
	}//namespace device
}//namespace base
