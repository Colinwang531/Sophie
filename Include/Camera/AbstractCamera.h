//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-01
//		Description :					摄像机抽象基类
//
//		History:						Author									Date										Description
//										王科威									2020-07-01									创建
//										王科威									2020-07-10									抽象摄像机类以提供数据结构
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
			//获取摄像机ID标识
			//@Return : 摄像机ID标识
			inline const std::string getCameraID(void) const
			{
				return cameraID;
			}

			//获取摄像机索引号
			//@Return : 摄像机索引号
			inline const int getCameraIndex(void) const
			{
				return cameraIndex;
			}

			//读/写摄像机IPv4地址
			//@enable : IPv4地址
			//@Return : IPv4地址
			inline void setIPAddress(const std::string ip)
			{
				cameraIP = ip;
			}
			inline const std::string getIPAddress(void) const
			{
				return cameraIP;
			}

			//读/写摄像机使能标识
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

		private:
			bool enableFlag;
			const int cameraIndex;
			const std::string cameraID;
			std::string cameraIP;
		};//class AbstractCamera
	}//namespace device
}//namespace base

#endif//BASE_DEVICE_ABSTRACT_CAMERA_H
