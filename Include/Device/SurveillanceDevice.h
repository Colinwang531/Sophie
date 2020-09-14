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
			//获取设备厂商类型
			//@Return : 设备厂商类型
			inline const SurveillanceDeviceFactory getDeviceFactory(void) const
			{
				return deviceFactory;
			}

			//获取设备类型
			//@Return : 设备类型
			inline const SurveillanceDeviceType getDeviceType(void) const
			{
				return deviceType;
			}

			//读/写设备IPv4地址
			//@ip : IPv4地址
			//@Return : IPv4地址
			inline void setDeviceIPv4Address(const std::string ip)
			{
				deviceIP = ip;
			}
			inline const std::string getDeviceIPv4Address(void) const
			{
				return deviceIP;
			}

			//读/写设备端口号
			//@ip : 端口号
			//@Return : 端口号
			inline void setDevicePortNumber(const unsigned short port = 0)
			{
				portNumber = port;
			}
			inline const unsigned short getDevicePortNumber(void) const
			{
				return portNumber;
			}

			//读/写设备登录用户名
			//@ip : 登录用户名
			//@Return : 登录用户名
			inline void setLoginUserName(const std::string name)
			{
				loginUserName = name;
			}
			inline const std::string getLoginUserName(void) const
			{
				return loginUserName;
			}

			//读/写设备登录密码
			//@ip : 登录密码
			//@Return : 登录密码
			inline void setLoginUserPassword(const std::string password)
			{
				loginUserPassword = password;
			}
			inline const std::string getLoginUserPassword(void) const
			{
				return loginUserPassword;
			}

			//获取设备摄像机信息
			//@cameras : 摄像机信息集合
			//@Return : 错误码
			virtual int getDeviceCamera(std::vector<AbstractCamera>& cameras);

		protected:
			//登录/注销设备
			//@Return : 当前设备实例总数
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
