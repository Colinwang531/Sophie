//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-07
//		Description :					�����豸��
//
//		History:						Author									Date										Description
//										������									2020-05-07									����
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
			//��ȡ��¼�û�ID��ʶ
			//@Return : ��¼�û�ID��ʶ
			inline const int getUserID(void) const
			{
				return userID;
			}

		protected:
			//����/ֹͣ�豸
			//@Return : ������
			int startDevice(void) override;
			int stopDevice(void) override;

			//��¼/�ǳ��豸
			//@Return : ������
			int loginDevice(void) override;
			int logoutDevice(void) override;

			//��ȡ�豸�������Ϣ
			//@cameras : �������Ϣ����
			//@Return : ������
			int getDeviceCamera(std::vector<AbstractCamera>& cameras) override;

			//�豸ʵʱԤ��ץͼ
			//@cameraIdx : �����������
			//@data : ͼƬ����
			//@bytes : ͼƬ�����С
			//@Return : ʵ��ͼƬ��С
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
