//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-01-20
//		Description :					设备抽象类
//
//		History:						Author									Date										Description
//										王科威									2021-01-20									创建
//

#ifndef FRAMEWORK_DEVICE_DEVICE_H
#define FRAMEWORK_DEVICE_DEVICE_H

#include <string>

namespace framework
{
	namespace device
	{
		typedef enum class tagFactoryType_t : int
		{
			FACTORY_TYPE_NONE = 0,
			FACTORY_TYPE_HK
		}FactoryType;

		typedef enum class tagDeviceType_t : int
		{
			DEVICE_TYPE_NONE = 0,
			DEVICE_TYPE_NVR
		}DeviceType;

		class Device
		{
		public:
			Device(
				const std::string id,
				const FactoryType ft = FactoryType::FACTORY_TYPE_NONE ,
				const DeviceType dt = DeviceType::DEVICE_TYPE_NONE);
			virtual ~Device(void);

		public:
			//获取设备ID标识
			//@Return : 设备ID标识
			inline const std::string getDeviceID(void) const
			{
				return did;
			}

			//获取设备厂商
			//@Return : 设备厂商
			inline const FactoryType getDeviceFactory(void) const
			{
				return dfactory;
			}

			//获取设备类型
			//@Return : 设备类型
			inline const DeviceType getDeviceType(void) const
			{
				return dtype;
			}

			//设置/获取设备使能标识
			//@enable : 使能标识
			//@Return : 使能标识
			inline void setEnable(const bool enable = false)
			{
				this->enable = enable;
			}
			inline const bool getEnable(void) const
			{
				return enable;
			}

		private:
			bool enable;
			const std::string did;
			const FactoryType dfactory;
			const DeviceType dtype;
		};//class Device
	}//namespace device
}//namespace framework

#endif//FRAMEWORK_DEVICE_DEVICE_H
