//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-01
//		Description :					组件抽象基类
//
//		History:						Author									Date										Description
//										王科威									2020-07-01									创建
//										王科威									2020-07-10									抽象设备类以提供数据结构
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
			//获取设备厂商类型
			//@Return : 设备厂商类型
			inline const DeviceFactory getDeviceFactory(void) const
			{
				return deviceFactory;
			}
			
			//获取设备类型
			//@Return : 设备类型
			inline const DeviceType getDeviceType(void) const
			{
				return deviceType;
			}

			//获取设备ID标识
			//@Return : 设备ID标识
			inline const std::string getDeviceID(void) const
			{
				return deviceID;
			}

			//读/写设备IPv4地址
			//@ip : 点分十进制IPv4地址
			//@Return : 点分十进制IPv4地址
			virtual void setDeviceIPv4Address(const std::string ip) = 0;
			virtual const std::string getDeviceIPv4Address(void) const = 0;

			//读/写设备端口号
			//@port : 端口号
			//@Return : 端口号
			virtual void setDevicePortNumber(const unsigned short port = 0) = 0;
			virtual const unsigned short getDevicePortNumber(void) const = 0;

			//读/写设备登录用户名
			//@name : 登录用户名
			//@Return : 登录用户名
			virtual void setLoginUserName(const std::string name) = 0;
			virtual const std::string getLoginUserName(void) const = 0;

			//读/写设备登录密码
			//@port : 登录密码
			//@Return : 登录密码
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
