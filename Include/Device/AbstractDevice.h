//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-07
//		Description :					物理设备抽象基类
//
//		History:						Author									Date										Description
//										王科威									2020-05-07									创建
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
			//设备创建和销毁接口
			//@Return : 错误码值
			virtual int createNewDevice(void);
			virtual int destoryDevice(void);

			//获取设备信息
			//@Return : 错误码值
			virtual int getDeviceConfig(void);

		protected:
			//设备创建和销毁计数器
			//需要初始化和反初始化的派生类使用该计数器作为判断条件
			static unsigned int gDeviceCounter;

		private:
			//共享读写锁
			//提供gDeviceCounter读写安全
			Locker wrl;
		};//class AbstractDevice
	}//namespace device
}//namespace base

#endif//BASE_ABSTRACT_DEVICE_H
