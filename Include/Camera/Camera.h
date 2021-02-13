//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-01-20
//		Description :					������������
//
//		History:						Author									Date										Description
//										������									2021-01-20									����
//

#ifndef FRAMEWORK_CAMERA_CAMERA_H
#define FRAMEWORK_CAMERA_CAMERA_H

#include <string>

namespace framework
{
	namespace device
	{
		class Camera
		{
		public:
			Camera(const int idx = -1);
			virtual ~Camera(void);

		public:
			//��ȡ�����ID��ʶ
			//@Return : �����ID��ʶ
			inline const std::string getID(void) const
			{
				return cid;
			}

			//��ȡ�����������
			//@Return : �����������
			inline const int getIndex(void) const
			{
				return index;
			}

			//����/��ȡ�����IPv4��ַ
			//@ipv4 : IPv4��ַ
			//@Return : IPv4��ַ
			inline void setIPv4(const std::string ipv4)
			{
				cip = ipv4;
			}
			inline const std::string getIPv4(void) const
			{
				return cip;
			}

			//����/��ȡ�豸ʹ�ܱ�ʶ
			//@enable : ʹ�ܱ�ʶ
			//@Return : ʹ�ܱ�ʶ
			inline void setEnable(const bool enable = false)
			{
				this->enable = enable;
			}
			inline const bool getEnable(void) const
			{
				return enable;
			}

		private:
			bool enable;
			const int index;
			const std::string cid;
			std::string cip;
		};//class Camera
	}//namespace device
}//namespace framework

#endif//FRAMEWORK_CAMERA_CAMERA_H
