//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-01-20
//		Description :					设备工厂类
//
//		History:						Author									Date										Description
//										王科威									2021-01-20									创建
//

#ifndef FRAMEWORK_DEVICE_DEVICE_FACTORY_H
#define FRAMEWORK_DEVICE_DEVICE_FACTORY_H

#include <vector>
#include "boost/serialization/singleton.hpp"
#include "boost/shared_ptr.hpp"
#include "Device/Device.h"
using Device = framework::device::Device;
using DevicePtr = boost::shared_ptr<Device>;

namespace framework
{
	namespace device
	{
		typedef struct tagCameraInfos_t 
		{
			bool enable;
			int type;//0模拟,1数字
			int idx;
			std::string id;
			std::string ipv4;
		}CameraInfos;

		class DeviceFactory
			: public boost::serialization::singleton<DeviceFactory>
		{
		public:
			DeviceFactory(void);
			~DeviceFactory(void);

		public:
			//创建设备
			//@id : 设备ID标识
			//@factory : 设备厂商
			//@type : 设备类型
			//@Return : 设备实例
			DevicePtr createNewDevice(
				const std::string id,
				const FactoryType ft = FactoryType::FACTORY_TYPE_NONE,
				const DeviceType dt = DeviceType::DEVICE_TYPE_NONE);

			//登录设备
			//@device : 设备
			//@username : 登录用户名
			//@userpwd : 登录密码
			//@ipv4 : 设备IPv4地址
			//@port : 设备端口号
			//@Return : 错误码
			int loginDevice(
				DevicePtr device,
				const std::string username,
				const std::string userpwd,
				const std::string ipv4,
				const unsigned short port = 0);

			//登出设备
			//@device : 设备
			//@Return : 错误码
			int logoutDevice(DevicePtr device);

			//获取摄像机信息
			//@device : 设备
			//@cinfos[out] : 摄像机信息
			//@Return : 错误码
			int getCameraInfos(DevicePtr device, std::vector<CameraInfos>& cinfos);
		};//class DeviceFactory
	}//namespace device
}//namespace framework

#endif//FRAMEWORK_DEVICE_DEVICE_FACTORY_H
