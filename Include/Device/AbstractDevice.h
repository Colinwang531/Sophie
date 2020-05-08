//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-07
//		Description :					�����豸�������
//
//		History:						Author									Date										Description
//										������									2020-05-07									����
//

#ifndef BASE_ABSTRACT_DEVICE_H
#define BASE_ABSTRACT_DEVICE_H

#include "Locker/Locker.h"
using Locker = base::locker::SharedMutex;

namespace base
{
	namespace device
	{
		class AbstractDevice
		{
		public:
			AbstractDevice(void);
			virtual ~AbstractDevice(void);

		public:
			//�豸���������ٽӿ�
			//@Return : ������ֵ
			virtual int createNewDevice(void);
			virtual int destoryDevice(void);

			//��ȡ�豸��Ϣ
			//@Return : ������ֵ
			virtual int getDeviceConfig(void);

		protected:
			//�豸���������ټ�����
			//��Ҫ��ʼ���ͷ���ʼ����������ʹ�øü�������Ϊ�ж�����
			static unsigned int gDeviceCounter;

		private:
			//�����д��
			//�ṩgDeviceCounter��д��ȫ
			Locker wrl;
		};//class AbstractDevice
	}//namespace device
}//namespace base

#endif//BASE_ABSTRACT_DEVICE_H
