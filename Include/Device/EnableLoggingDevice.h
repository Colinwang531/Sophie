//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-07
//		Description :					�����豸��¼/�ǳ�������
//
//		History:						Author									Date										Description
//										������									2020-05-07									����
//

#ifndef ENABLE_LOGGING_DEVICE_H
#define ENABLE_LOGGING_DEVICE_H

namespace base
{
	namespace device
	{
		class EnableLoggingDevice
		{
		protected:
			EnableLoggingDevice(void) {}
			virtual ~EnableLoggingDevice(void) {}

		protected:
			//�豸��¼�͵ǳ��ӿ�
			//�̳������ʵ�ֵ�¼�͵ǳ��ӿ�
			//@Return : ������ֵ
			virtual int loginDevice(void) = 0;
			virtual int logoutDevice(void) = 0;
		};//class EnableLoggingDevice
	}//namespace device
}//namespace base

#endif//ENABLE_LOGGING_DEVICE_H
