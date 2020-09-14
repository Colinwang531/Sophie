//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-10
//		Description :					��������豸������
//
//		History:						Author									Date										Description
//										������									2020-07-10									����
//

#ifndef BASE_DEVICE_SURVEILLANCE_DEVICE_H
#define BASE_DEVICE_SURVEILLANCE_DEVICE_H

#include <vector>
#include "Camera/AbstractCamera.h"
#include "Device/AbstractDevice.h"

namespace base
{
	namespace device
	{
		typedef enum class tagSurveillanceDeviceFactory_t : int
		{
			SURVEILLANCE_DEVICE_FACTORY_NONE = 0,
			SURVEILLANCE_DEVICE_FACTORY_HK = 1,
			SURVEILLANCE_DEVICE_FACTORY_DH = 2,
			SURVEILLANCE_DEVICE_FACTORY_ET = 3
		}SurveillanceDeviceFactory;

		typedef enum class tagSurveillanceDeviceType_t : int
		{
			SURVEILLANCE_DEVICE_TYPE_NONE = 0,
			SURVEILLANCE_DEVICE_TYPE_DVR = 1,
			SURVEILLANCE_DEVICE_TYPE_NVR = 2,
			SURVEILLANCE_DEVICE_TYPE_IPC = 3
		}SurveillanceDeviceType;

		class SurveillanceDevice : public AbstractDevice
		{
		public:
			SurveillanceDevice(
				const std::string did, 
				const SurveillanceDeviceFactory factory = SurveillanceDeviceFactory::SURVEILLANCE_DEVICE_FACTORY_NONE,
				const SurveillanceDeviceType type = SurveillanceDeviceType::SURVEILLANCE_DEVICE_TYPE_NONE);
			virtual ~SurveillanceDevice(void);

		public:
			//��ȡ�豸��������
			//@Return : �豸��������
			inline const SurveillanceDeviceFactory getDeviceFactory(void) const
			{
				return deviceFactory;
			}

			//��ȡ�豸����
			//@Return : �豸����
			inline const SurveillanceDeviceType getDeviceType(void) const
			{
				return deviceType;
			}

			//��/д�豸IPv4��ַ
			//@ip : IPv4��ַ
			//@Return : IPv4��ַ
			inline void setDeviceIPv4Address(const std::string ip)
			{
				deviceIP = ip;
			}
			inline const std::string getDeviceIPv4Address(void) const
			{
				return deviceIP;
			}

			//��/д�豸�˿ں�
			//@ip : �˿ں�
			//@Return : �˿ں�
			inline void setDevicePortNumber(const unsigned short port = 0)
			{
				portNumber = port;
			}
			inline const unsigned short getDevicePortNumber(void) const
			{
				return portNumber;
			}

			//��/д�豸��¼�û���
			//@ip : ��¼�û���
			//@Return : ��¼�û���
			inline void setLoginUserName(const std::string name)
			{
				loginUserName = name;
			}
			inline const std::string getLoginUserName(void) const
			{
				return loginUserName;
			}

			//��/д�豸��¼����
			//@ip : ��¼����
			//@Return : ��¼����
			inline void setLoginUserPassword(const std::string password)
			{
				loginUserPassword = password;
			}
			inline const std::string getLoginUserPassword(void) const
			{
				return loginUserPassword;
			}

			//��ȡ�豸�������Ϣ
			//@cameras : �������Ϣ����
			//@Return : ������
			virtual int getDeviceCamera(std::vector<AbstractCamera>& cameras);

		protected:
			//��¼/ע���豸
			//@Return : ��ǰ�豸ʵ������
			virtual int loginDevice(void);
			virtual int logoutDevice(void);

		private:
			const SurveillanceDeviceFactory deviceFactory;
			const SurveillanceDeviceType deviceType;

			unsigned short portNumber;
			std::string deviceIP;
			std::string loginUserName;
			std::string loginUserPassword;
			static int gDeviceCounter;
		};//class SurveillanceDevice
	}//namespace device
}//namespace base

#endif//BASE_DEVICE_SURVEILLANCE_DEVICE_H
