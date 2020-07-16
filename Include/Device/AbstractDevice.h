//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-01
//		Description :					����������
//
//		History:						Author									Date										Description
//										������									2020-07-01									����
//										������									2020-07-10									�����豸�����ṩ���ݽṹ
//

#ifndef BASE_DEVICE_ABSTRACT_DEVICE_H
#define BASE_DEVICE_ABSTRACT_DEVICE_H

#include "boost/noncopyable.hpp"

namespace base
{
	namespace device
	{
		typedef enum class tagDeviceFactory_t : int
		{
			DEVICE_FACTORY_NONE = 0,
			DEVICE_FACTORY_HK = 1,
			DEVICE_FACTORY_DH = 2,
			DEVICE_FACTORY_ET = 3
		}DeviceFactory;

		typedef enum class tagDeviceType_t : int
		{
			DEVICE_TYPE_NONE = 0,
			DEVICE_TYPE_DVR = 1,
			DEVICE_TYPE_NVR = 2,
			DEVICE_TYPE_IPC = 3
		}DeviceType;

		class AbstractDevice : private boost::noncopyable
		{
		public:
			AbstractDevice(
				const std::string id,
				const DeviceFactory factory = DeviceFactory::DEVICE_FACTORY_NONE, 
				const DeviceType type = DeviceType::DEVICE_TYPE_NONE);
			virtual ~AbstractDevice(void);

		public:
			//��ȡ�豸��������
			//@Return : �豸��������
			inline const DeviceFactory getDeviceFactory(void) const
			{
				return deviceFactory;
			}
			
			//��ȡ�豸����
			//@Return : �豸����
			inline const DeviceType getDeviceType(void) const
			{
				return deviceType;
			}

			//��ȡ�豸ID��ʶ
			//@Return : �豸ID��ʶ
			inline const std::string getDeviceID(void) const
			{
				return deviceID;
			}

			//��/д�豸IPv4��ַ
			//@ip : ���ʮ����IPv4��ַ
			//@Return : ���ʮ����IPv4��ַ
			virtual void setDeviceIPv4Address(const std::string ip) = 0;
			virtual const std::string getDeviceIPv4Address(void) const = 0;

			//��/д�豸�˿ں�
			//@port : �˿ں�
			//@Return : �˿ں�
			virtual void setDevicePortNumber(const unsigned short port = 0) = 0;
			virtual const unsigned short getDevicePortNumber(void) const = 0;

			//��/д�豸��¼�û���
			//@name : ��¼�û���
			//@Return : ��¼�û���
			virtual void setLoginUserName(const std::string name) = 0;
			virtual const std::string getLoginUserName(void) const = 0;

			//��/д�豸��¼����
			//@port : ��¼����
			//@Return : ��¼����
			virtual void setLoginUserPassword(const std::string password) = 0;
			virtual const std::string getLoginUserPassword(void) const = 0;

		private:
			const std::string deviceID;
			const DeviceFactory deviceFactory;
			const DeviceType deviceType;
		};//class AbstractDevice
	}//namespace device
}//namespace base

#endif//BASE_DEVICE_ABSTRACT_DEVICE_H
