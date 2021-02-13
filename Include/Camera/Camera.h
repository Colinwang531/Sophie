//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-01-20
//		Description :					摄像机抽象基类
//
//		History:						Author									Date										Description
//										王科威									2021-01-20									创建
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
			//获取摄像机ID标识
			//@Return : 摄像机ID标识
			inline const std::string getID(void) const
			{
				return cid;
			}

			//获取摄像机索引号
			//@Return : 摄像机索引号
			inline const int getIndex(void) const
			{
				return index;
			}

			//设置/获取摄像机IPv4地址
			//@ipv4 : IPv4地址
			//@Return : IPv4地址
			inline void setIPv4(const std::string ipv4)
			{
				cip = ipv4;
			}
			inline const std::string getIPv4(void) const
			{
				return cip;
			}

			//设置/获取设备使能标识
			//@enable : 使能标识
			//@Return : 使能标识
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
