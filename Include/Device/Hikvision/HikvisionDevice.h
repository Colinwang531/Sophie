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

#ifndef HIKVISION_DEVICE_H
#define HIKVISION_DEVICE_H

#include "Device/AbstractDevice.h"
#include "Device/EnableLoggingDevice.h"

namespace base
{
	namespace device
	{
		class HikvisionDevice : public AbstractDevice, protected EnableLoggingDevice
		{
		public:
			HikvisionDevice(
				const std::string name, const std::string pwd, const std::string address, const unsigned short port = 8000);
			virtual ~HikvisionDevice(void);

		public:
			int createNewDevice(void) override;
			int destoryDevice(void) override;
			int getDeviceConfig(void) override;

			int loginDevice(void) override;
			int logoutDevice(void) override;

		private:
			const std::string userName;
			const std::string userPassword;
			const std::string deviceIP;
			const unsigned short devicePort;
			int userID;
		};//class HikvisionDevice
	}//namespace device
}//namespace base

#endif//HIKVISION_DEVICE_H
