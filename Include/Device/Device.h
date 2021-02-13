//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-01-20
//		Description :					�豸������
//
//		History:						Author									Date										Description
//										������									2021-01-20									����
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
			//��ȡ�豸ID��ʶ
			//@Return : �豸ID��ʶ
			inline const std::string getDeviceID(void) const
			{
				return did;
			}

			//��ȡ�豸����
			//@Return : �豸����
			inline const FactoryType getDeviceFactory(void) const
			{
				return dfactory;
			}

			//��ȡ�豸����
			//@Return : �豸����
			inline const DeviceType getDeviceType(void) const
			{
				return dtype;
			}

			//����/��ȡ�豸ʹ�ܱ�ʶ
			//@enable : ʹ�ܱ�ʶ
			//@Return : ʹ�ܱ�ʶ
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
