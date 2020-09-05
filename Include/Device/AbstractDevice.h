//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-01
//		Description :					组件抽象基类
//
//		History:						Author									Date										Description
//										王科威									2020-07-01									创建
//										王科威									2020-07-10									抽象设备类以提供数据结构
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
			//获取设备ID标识
			//@Return : 设备ID标识
			inline const std::string getDeviceID(void) const
			{
				return deviceID;
			}

			//读/写设备使能标识
			//@enable : 使能标识
			//@Return : 使能标识
			inline void setEnableFlag(const bool enable = false)
			{
				enableFlag = enable;
			}
			inline const bool getEnableFlag(void) const
			{
				return enableFlag;
			}

			//启动/停止设备
			//@Return : 错误码
			virtual int startDevice(void);
			virtual int stopDevice(void);

		private:
			bool enableFlag;
			const std::string deviceID;
		};//class AbstractDevice
	}//namespace device
}//namespace base

#endif//BASE_DEVICE_ABSTRACT_DEVICE_H
