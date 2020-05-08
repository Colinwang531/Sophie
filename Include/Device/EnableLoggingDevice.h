//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-07
//		Description :					物理设备登录/登出抽象类
//
//		History:						Author									Date										Description
//										王科威									2020-05-07									创建
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
			//设备登录和登出接口
			//继承类必须实现登录和登出接口
			//@Return : 错误码值
			virtual int loginDevice(void) = 0;
			virtual int logoutDevice(void) = 0;
		};//class EnableLoggingDevice
	}//namespace device
}//namespace base

#endif//ENABLE_LOGGING_DEVICE_H
