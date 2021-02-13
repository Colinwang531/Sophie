//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-01-20
//		Description :					海康摄像机类
//
//		History:						Author									Date										Description
//										王科威									2020-05-07									创建
//

#ifndef FRAMEWORK_DEVICE_HIKVISION_CAMERA_H
#define FRAMEWORK_DEVICE_HIKVISION_CAMERA_H

#include "boost/shared_ptr.hpp"
#include "CH-HCNetSDKV6.1.4.17_build20200331_Linux64/HCNetSDK.h"
#include "Network/ASIO/Session/TCPSession.h"
using TCPSession = framework::network::asio::TCPSession;
using TCPSessionPtr = boost::shared_ptr<TCPSession>;
#include "Camera/Camera.h"

namespace framework
{
	namespace device
	{
		class HikvisionCamera : public Camera
		{
		public:
			HikvisionCamera(
				const int idx = -1);
			virtual ~HikvisionCamera(void);

		public:
			int openStream(
				TCPSessionPtr sp, const int userID = -1);
			int closeStream(void);

		private:
			int sendData(BYTE* pBuffer = nullptr, DWORD dwBufSize = 0, bool head = false);
			static void CALLBACK livestreamDataCaptureCallback(
				LONG lPlayHandle, DWORD dwDataType, BYTE* pBuffer, DWORD dwBufSize, void* pUser);

		private:
			int streamID;
			TCPSessionPtr tsp;
		};//class HikvisionCamera
	}//namespace device
}//namespace framework

#endif//FRAMEWORK_DEVICE_HIKVISION_CAMERA_H
