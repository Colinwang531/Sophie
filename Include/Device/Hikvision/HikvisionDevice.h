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

#ifndef BASE_DEVICE_HIKVISION_DEVICE_H
#define BASE_DEVICE_HIKVISION_DEVICE_H

#include "Device/SurveillanceDevice.h"

namespace base
{
	namespace device
	{
		class HikvisionDevice : public SurveillanceDevice
		{
		public:
			HikvisionDevice(
				const std::string did, 
				const SurveillanceDeviceType type = SurveillanceDeviceType::SURVEILLANCE_DEVICE_TYPE_NONE);
			virtual ~HikvisionDevice(void);

		public:
			//获取登录用户ID标识
			//@Return : 登录用户ID标识
			inline const int getUserID(void) const
			{
				return userID;
			}

		protected:
			//启动/停止设备
			//@Return : 错误码
			int startDevice(void) override;
			int stopDevice(void) override;

			//登录/登出设备
			//@Return : 错误码
			int loginDevice(void) override;
			int logoutDevice(void) override;

			//获取设备摄像机信息
			//@cameras : 摄像机信息集合
			//@Return : 错误码
			int getDeviceCamera(std::vector<AbstractCamera>& cameras) override;

			//设备实时预览抓图
			//@cameraIdx : 摄像机索引号
			//@data : 图片缓存
			//@bytes : 图片缓存大小
			//@Return : 实际图片大小
			int captureRealplayJPEGImage(
				const int cameraIdx = -1,
				char* data = nullptr,
				const int bytes = 0);

		private:
			int userID;
		};//class HikvisionDevice
	}//namespace device
}//namespace base

#endif//BASE_DEVICE_HIKVISION_DEVICE_H
