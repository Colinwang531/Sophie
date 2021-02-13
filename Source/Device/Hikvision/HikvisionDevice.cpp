#include "boost/checked_delete.hpp"
#include "boost/make_shared.hpp"
#include "boost/filesystem/path.hpp" 
#include "boost/filesystem/operations.hpp"
#include "Error.h"
#include "Define.h"
#include "Camera/Hikvision/HikvisionCamera.h"
#include "Device/Hikvision/HikvisionDevice.h"

namespace framework
{
	namespace device
	{
		int HikvisionDevice::loginUserCount = 0;

		HikvisionDevice::HikvisionDevice(
			const std::string id, 
			const DeviceType dt /* = DeviceType::DEVICE_TYPE_NONE */)
			: Device(id, FactoryType::FACTORY_TYPE_HK, dt), loginID{ -1 }
		{}
		HikvisionDevice::~HikvisionDevice()
		{}

		int HikvisionDevice::loginDevice(
			const std::string username, 
			const std::string userpwd, 
			const std::string ipv4, 
			const unsigned short port /* = 0 */)
		{
			int e{ eSuccess };

			if (1 == ++loginUserCount)
			{
// 				const std::string exePath{
// 					boost::filesystem::initial_path<boost::filesystem::path>().string() };
// 				NET_DVR_LOCAL_SDK_PATH path{ 0 };
// 				sprintf(path.sPath, exePath.c_str());
// 				e = NET_DVR_SetSDKInitCfg(NET_SDK_INIT_CFG_SDK_PATH, &path);
				e = NET_DVR_Init() ? eSuccess : eBadInitSDK;
			}

			if (eSuccess == e)
			{
				NET_DVR_USER_LOGIN_INFO loginInfo{ 0 };
				loginInfo.bUseAsynLogin = 0;
				strcpy(loginInfo.sDeviceAddress, ipv4.c_str());
				loginInfo.wPort = port;
				strcpy(loginInfo.sUserName, username.c_str());
				strcpy(loginInfo.sPassword, userpwd.c_str());
				NET_DVR_DEVICEINFO_V40 deviceInfo{ 0 };

				//同步登录
				loginID = NET_DVR_Login_V40(&loginInfo, &deviceInfo);
				e = -1 < loginID ? eSuccess : eBadLoginDevice;

				if (eSuccess == e)
				{
					initCameras();
				}
			}

			return e;
		}

		int HikvisionDevice::logoutDevice()
		{
			int e{ -1 < loginID ? eSuccess : eBadOperate };

			if (eSuccess == e)
			{
				e = NET_DVR_Logout(loginID) ? eSuccess : eBadLogoutDevice;

// 				for (int i = 0; i != 64; ++i)
// 				{
// 					boost::checked_delete(cameras[i]);
// 				}

				if (eSuccess == e)
				{
					if (0 == --loginUserCount)
					{
						e = NET_DVR_Cleanup() ? eSuccess : eBadCleanupSDK;
					}
				}
			}

			return e;
		}

		void HikvisionDevice::initCameras()
		{
			int e{ -1 < loginID ? eSuccess : eBadOperate };

			if (eSuccess == e)
			{
				LONG groupNo = 0;
				DWORD dwRet{ 0 };
				NET_DVR_IPPARACFG_V40 IPAccessCfgV40{ 0 };

				if (NET_DVR_GetDVRConfig(loginID, NET_DVR_GET_IPPARACFG_V40, 0, &IPAccessCfgV40, sizeof(NET_DVR_IPPARACFG_V40), &dwRet))
				{
					for (DWORD i = 0; i < IPAccessCfgV40.dwDChanNum; i++)
					{
						switch (IPAccessCfgV40.struStreamMode[i].byGetStreamType)
						{
						case 0:
						{
							if (IPAccessCfgV40.struStreamMode[i].uGetStream.struChanInfo.byEnable)
							{
								BYTE byIPID{ IPAccessCfgV40.struStreamMode[i].uGetStream.struChanInfo.byIPID };
								BYTE byIPIDHigh{ IPAccessCfgV40.struStreamMode[i].uGetStream.struChanInfo.byIPIDHigh };
								int iDevInfoIndex{ byIPIDHigh * 256 + byIPID - 1 - groupNo * 64 };

								cameras[IPAccessCfgV40.dwStartDChan + i] = boost::make_shared<HikvisionCamera>(IPAccessCfgV40.dwStartDChan + i);
								cameras[IPAccessCfgV40.dwStartDChan + i]->setIPv4(IPAccessCfgV40.struIPDevInfo[iDevInfoIndex].struIP.sIpV4);
							}

							break;
						}
						default:
							break;
						}
					}
				}
			}
		}

// 		int HikvisionDevice::captureRealplayJPEGImage(
// 			const int cameraIdx /* = -1 */,
// 			char* data /* = nullptr */,
// 			const int bytes /* = 0 */)
// 		{
// 			DWORD picsize{ 0 };
// 
// 			if (-1 < userID && -1 < cameraIdx && data && 0 < bytes)
// 			{
// 				//默认抓取1080p/最佳质量图片
// 				NET_DVR_JPEGPARA jpegParam{ 9, 0 };
// 				NET_DVR_CaptureJPEGPicture_NEW(userID, cameraIdx, &jpegParam, data, bytes, &picsize);
// 			}
// 
// 			return SurveillanceDevice::captureRealplayJPEGImage(cameraIdx, data, picsize);
// 		}
	}//namespace device
}//namespace base
