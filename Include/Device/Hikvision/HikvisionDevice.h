//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-07
//		Description :					海康设备类
//
//		History:						Author									Date										Description
//										王科威									2020-05-07									创建
//

#ifndef BASE_DEVICE_HIKVISION_DEVICE_H
#define BASE_DEVICE_HIKVISION_DEVICE_H

#include "Device/SurveillanceDevice.h"

namespace base
{
	namespace device
	{
		class HikvisionDevice : public SurveillanceDevice
		{
		public:
			HikvisionDevice(
				const std::string id, 
				const DeviceType type = DeviceType::DEVICE_TYPE_NONE);
			virtual ~HikvisionDevice(void);

		private:
			int userID;
		};//class HikvisionDevice
	}//namespace device
}//namespace base

#endif//BASE_DEVICE_HIKVISION_DEVICE_H
