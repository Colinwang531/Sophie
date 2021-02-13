//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-01-20
//		Description :					�����豸��
//
//		History:						Author									Date										Description
//										������									2020-05-07									����
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
			//��ȡ��¼ID��ʶ
			//@Return : ��¼ID��ʶ
			inline const int getLoginID(void) const
			{
				return loginID;
			}

			//��¼/�ǳ��豸
			//@Return : ������
			int loginDevice(
				const std::string username, 
				const std::string userpwd, 
				const std::string ipv4, 
				const unsigned short port = 0);
			int logoutDevice(void);

			//��ȡ�������Ϣ
			//@cameras : �������Ϣ����
			//@Return : ������
			inline CameraPtr getCamera(const int idx = -1) const 
			{
				return -1 < idx && 64 > idx ? cameras[idx] : nullptr;
			}

			//�豸ʵʱԤ��ץͼ
			//@cameraIdx : �����������
			//@data : ͼƬ����
			//@bytes : ͼƬ�����С
			//@Return : ʵ��ͼƬ��С
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
