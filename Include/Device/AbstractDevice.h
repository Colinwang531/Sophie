//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-01
//		Description :					����������
//
//		History:						Author									Date										Description
//										������									2020-07-01									����
//										������									2020-07-10									�����豸�����ṩ���ݽṹ
//

#ifndef BASE_DEVICE_ABSTRACT_DEVICE_H
#define BASE_DEVICE_ABSTRACT_DEVICE_H

#include <string>
#include "boost/noncopyable.hpp"

namespace base
{
	namespace device
	{
		class AbstractDevice : private boost::noncopyable
		{
		public:
			AbstractDevice(const std::string did);
			virtual ~AbstractDevice(void);

		public:
			//��ȡ�豸ID��ʶ
			//@Return : �豸ID��ʶ
			inline const std::string getDeviceID(void) const
			{
				return deviceID;
			}

			//��/д�豸ʹ�ܱ�ʶ
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

			//����/ֹͣ�豸
			//@Return : ������
			virtual int startDevice(void);
			virtual int stopDevice(void);

		private:
			bool enableFlag;
			const std::string deviceID;
		};//class AbstractDevice
	}//namespace device
}//namespace base

#endif//BASE_DEVICE_ABSTRACT_DEVICE_H
