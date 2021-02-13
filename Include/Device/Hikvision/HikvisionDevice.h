//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-01-20
//		Description :					海康设备类
//
//		History:						Author									Date										Description
//										王科威									2020-05-07									创建
//

#ifndef FRAMEWORK_DEVICE_HIKVISION_DEVICE_H
#define FRAMEWORK_DEVICE_HIKVISION_DEVICE_H

#include "boost/shared_ptr.hpp"
#include "Camera/Camera.h"
using Camera = framework::device::Camera;
using CameraPtr = boost::shared_ptr<Camera>;
#include "Device/Device.h"

namespace framework
{
	namespace device
	{
		class HikvisionDevice : public Device
		{
		public:
			HikvisionDevice(
				const std::string id,
				const DeviceType dt = DeviceType::DEVICE_TYPE_NONE);
			virtual ~HikvisionDevice(void);

		public:
			//获取登录ID标识
			//@Return : 登录ID标识
			inline const int getLoginID(void) const
			{
				return loginID;
			}

			//登录/登出设备
			//@Return : 错误码
			int loginDevice(
				const std::string username, 
				const std::string userpwd, 
				const std::string ipv4, 
				const unsigned short port = 0);
			int logoutDevice(void);

			//获取摄像机信息
			//@cameras : 摄像机信息集合
			//@Return : 错误码
			inline CameraPtr getCamera(const int idx = -1) const 
			{
				return -1 < idx && 64 > idx ? cameras[idx] : nullptr;
			}

			//设备实时预览抓图
			//@cameraIdx : 摄像机索引号
			//@data : 图片缓存
			//@bytes : 图片缓存大小
			//@Return : 实际图片大小
// 			int captureRealplayJPEGImage(
// 				const int cameraIdx = -1,
// 				char* data = nullptr,
// 				const int bytes = 0);

		private:
			void initCameras(void);

		private:
			static int loginUserCount;
			int loginID;
			CameraPtr cameras[64];
		};//class HikvisionDevice
	}//namespace device
}//namespace framework

#endif//FRAMEWORK_DEVICE_HIKVISION_DEVICE_H
