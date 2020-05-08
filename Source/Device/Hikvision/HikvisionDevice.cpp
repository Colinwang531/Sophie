#include "CH-HCNetSDKV6.1.4.17_build20200331_Linux64/HCNetSDK.h"
#include "Define.h"
#include "Error.h"
#include "Device/Hikvision/HikvisionDevice.h"

namespace base
{
	namespace device
	{
		HikvisionDevice::HikvisionDevice(
			const std::string name, const std::string pwd, const std::string address, const unsigned short port /* = 8000 */)
			: AbstractDevice(), userName{ name }, userPassword{ pwd }, deviceIP{ address }, devicePort{ port }, userID{ -1 }
		{}

		HikvisionDevice::~HikvisionDevice()
		{}

		int HikvisionDevice::createNewDevice()
		{
			int e{ eSuccess };

			if (0 == AbstractDevice::gDeviceCounter)
			{
				e = NET_DVR_Init() ? eSuccess : eBadInitSDK;
			}

			if (eSuccess == e)
			{
				AbstractDevice::createNewDevice();
				e = loginDevice();
			}

			return e;
		}

		int HikvisionDevice::destoryDevice()
		{
			int e{ logoutDevice() };

			if (eSuccess == e)
			{
				AbstractDevice::destoryDevice();
			}

			if (0 == AbstractDevice::gDeviceCounter)
			{
				e = NET_DVR_Cleanup() ? eSuccess : eBadCleanupSDK;
			}

			return e;
		}

		int HikvisionDevice::getDeviceConfig()
		{
			int e{ -1 < userID ? eSuccess : eBadOperate };

			if (eSuccess == e)
			{
				DWORD dwRet{ 0 };
				NET_DVR_IPPARACFG_V40 IPAccessCfgV40{ 0 };
				NET_DVR_DEVICEINFO_V40 deviceInfo{ 0 };

				if (NET_DVR_GetDVRConfig(userID, NET_DVR_GET_IPPARACFG_V40, 0, &IPAccessCfgV40, sizeof(NET_DVR_IPPARACFG_V40), &dwRet))
				{
					for (DWORD i = 0; i < IPAccessCfgV40.dwDChanNum; i++)
					{
						switch (IPAccessCfgV40.struStreamMode[i].byGetStreamType)
						{
							case 0:
							{
								if (IPAccessCfgV40.struStreamMode[i].uGetStream.struChanInfo.byEnable)
								{
// 									BYTE byIPID{ IPAccessCfgV40.struStreamMode[i].uGetStream.struChanInfo.byIPID };
// 									BYTE byIPIDHigh{ IPAccessCfgV40.struStreamMode[i].uGetStream.struChanInfo.byIPIDHigh };
// 									int iDevInfoIndex{ byIPIDHigh * 256 + byIPID - 1 - groupNo * 64 };
								}

								break;
							}
							default:
								break;
						}
					}
				}
				else
				{
					e = eNotSupport;
				}
			}

			return e;
		}

		int HikvisionDevice::loginDevice()
		{
			int e{ 
				userName.empty() || userPassword.empty() || deviceIP.empty() || 
				minPortNumber > devicePort || maxPortNumber < devicePort || -1 < userID
				? eBadOperate : eSuccess };

			if (eSuccess == e)
			{
				NET_DVR_USER_LOGIN_INFO loginInfo{ 0 };
				loginInfo.bUseAsynLogin = 0;
				strcpy(loginInfo.sDeviceAddress, deviceIP.c_str());
				loginInfo.wPort = 8000;
				strcpy(loginInfo.sUserName, userName.c_str());
				strcpy(loginInfo.sPassword, userPassword.c_str());
				NET_DVR_DEVICEINFO_V40 deviceInfo{ 0 };

				//Í¬²½µÇÂ¼
				userID = NET_DVR_Login_V40(&loginInfo, &deviceInfo);
				e = -1 < userID ? eSuccess : eBadLoginDevice;
			}

			return e;
		}

		int HikvisionDevice::logoutDevice()
		{
			int e{ -1 < userID ? eSuccess : eBadOperate };

			if (eSuccess == e)
			{
				e = NET_DVR_Logout(userID) ? eSuccess : eBadLogoutDevice;
			}

			return e;
		}
	}//namespace device
}//namespace base
