//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-01
//		Description :					������������
//
//		History:						Author									Date										Description
//										������									2020-07-01									����
//										������									2020-07-10									��������������ṩ���ݽṹ
//

#ifndef BASE_DEVICE_ABSTRACT_CAMERA_H
#define BASE_DEVICE_ABSTRACT_CAMERA_H

#include <string>

namespace base
{
	namespace device
	{
		class AbstractCamera
		{
		public:
			AbstractCamera(const int idx = -1);
			virtual ~AbstractCamera(void);

		public:
			//��ȡ�����ID��ʶ
			//@Return : �����ID��ʶ
			inline const std::string getCameraID(void) const
			{
				return cameraID;
			}

			//��ȡ�����������
			//@Return : �����������
			inline const int getCameraIndex(void) const
			{
				return cameraIndex;
			}

			//��/д�����IPv4��ַ
			//@enable : IPv4��ַ
			//@Return : IPv4��ַ
			inline void setIPAddress(const std::string ip)
			{
				cameraIP = ip;
			}
			inline const std::string getIPAddress(void) const
			{
				return cameraIP;
			}

			//��/д�����ʹ�ܱ�ʶ
			//@enable : ʹ�ܱ�ʶ
			//@Return : ʹ�ܱ�ʶ
			inline void setEnableFlag(const bool enable = false)
			{
				enableFlag = enable;
			}
			inline const bool getEnableFlag(void) const
			{
				return enableFlag;
			}

		private:
			bool enableFlag;
			const int cameraIndex;
			const std::string cameraID;
			std::string cameraIP;
		};//class AbstractCamera
	}//namespace device
}//namespace base

#endif//BASE_DEVICE_ABSTRACT_CAMERA_H
