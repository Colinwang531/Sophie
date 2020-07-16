//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-10
//		Description :					安防监控设备抽象类
//
//		History:						Author									Date										Description
//										王科威									2020-07-10									创建
//

#ifndef BASE_DEVICE_SURVEILLANCE_DEVICE_H
#define BASE_DEVICE_SURVEILLANCE_DEVICE_H

#include "Device/AbstractDevice.h"

namespace base
{
	namespace device
	{
		class SurveillanceDevice : public AbstractDevice
		{
		public:
			SurveillanceDevice(
				const std::string id, 
				const DeviceFactory factory = DeviceFactory::DEVICE_FACTORY_NONE,
				const DeviceType type = DeviceType::DEVICE_TYPE_NONE);
			virtual ~SurveillanceDevice(void);

		public:
			inline void setDeviceIPv4Address(const std::string ip) override
			{
				deviceIP = ip;
			}

			inline const std::string getDeviceIPv4Address(void) const override
			{
				return deviceIP;
			}

			inline void setDevicePortNumber(const unsigned short port = 0) override
			{
				portNumber = port;
			}

			inline const unsigned short getDevicePortNumber(void) const override
			{
				return portNumber;
			}

			inline void setLoginUserName(const std::string name) override
			{
				loginUserName = name;
			}

			inline const std::string getLoginUserName(void) const override
			{
				return loginUserName;
			}

			inline void setLoginUserPassword(const std::string password) override
			{
				loginUserPassword = password;
			}

			inline const std::string getLoginUserPassword(void) const override
			{
				return loginUserPassword;
			}

		private:
			unsigned short portNumber;
			std::string deviceIP;
			std::string loginUserName;
			std::string loginUserPassword;
		};//class SurveillanceDevice
	}//namespace device
}//namespace base

#endif//BASE_DEVICE_SURVEILLANCE_DEVICE_H
