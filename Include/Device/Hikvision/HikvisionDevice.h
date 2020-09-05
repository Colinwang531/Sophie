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
				const std::string did, 
				const SurveillanceDeviceType type = SurveillanceDeviceType::SURVEILLANCE_DEVICE_TYPE_NONE);
			virtual ~HikvisionDevice(void);

		protected:
			//启动/停止设备
			//@Return : 错误码
			int startDevice(void) override;
			int stopDevice(void) override;

			//登录/登出设备
			//@Return : 错误码
			int loginDevice(void) override;
			int logoutDevice(void) override;

		private:
			int userID;
		};//class HikvisionDevice
	}//namespace device
}//namespace base

#endif//BASE_DEVICE_HIKVISION_DEVICE_H
