#include <string.h>
#include "CH-HCNetSDKV6.1.4.17_build20200331_Linux64/HCNetSDK.h"
#include "Error.h"
#include "Define.h"
#include "Device/Hikvision/HikvisionDevice.h"

namespace base
{
	namespace device
	{
		HikvisionDevice::HikvisionDevice(
			const std::string did,
			const SurveillanceDeviceType type /* = SurveillanceDeviceType::SURVEILLANCE_DEVICE_TYPE_NONE */)
			: SurveillanceDevice(did, SurveillanceDeviceFactory::SURVEILLANCE_DEVICE_FACTORY_HK, type), userID{ -1 }
		{}

		HikvisionDevice::~HikvisionDevice()
		{}

		int HikvisionDevice::startDevice()
		{
			return loginDevice();
		}

		int HikvisionDevice::stopDevice()
		{
			return logoutDevice();
		}

		int HikvisionDevice::loginDevice()
		{
			const std::string ip{ getDeviceIPv4Address() }, name{ getLoginUserName() }, password{ getLoginUserPassword() };
			const unsigned short port{ getDevicePortNumber() };
			int e{ 
				name.empty() || password.empty() || ip.empty() || 
				gMinPortNumber > port || gMaxPortNumber < port || -1 < userID
				? eBadOperate : eSuccess };

			if (eSuccess == e)
			{
				if (1 == SurveillanceDevice::loginDevice())
				{
					e = NET_DVR_Init() ? eSuccess : eBadInitSDK;
				}

				if (eSuccess == e)
				{
					NET_DVR_USER_LOGIN_INFO loginInfo{ 0 };
					loginInfo.bUseAsynLogin = 0;
					strcpy(loginInfo.sDeviceAddress, ip.c_str());
					loginInfo.wPort = 8000;
					strcpy(loginInfo.sUserName, name.c_str());
					strcpy(loginInfo.sPassword, password.c_str());
					NET_DVR_DEVICEINFO_V40 deviceInfo{ 0 };

					//Í¬²½µÇÂ¼
					userID = NET_DVR_Login_V40(&loginInfo, &deviceInfo);
					e = -1 < userID ? eSuccess : eBadLoginDevice;
				}
			}

			return e;
		}

		int HikvisionDevice::logoutDevice()
		{
			int e{ -1 < userID ? eSuccess : eBadOperate };

			if (eSuccess == e)
			{
				e = NET_DVR_Logout(userID) ? eSuccess : eBadLogoutDevice;

				if (eSuccess == e)
				{
					if (0 == SurveillanceDevice::logoutDevice())
					{
						e = NET_DVR_Cleanup() ? eSuccess : eBadCleanupSDK;
					}
				}
			}

			return e;
		}

		int HikvisionDevice::getDeviceCamera(std::vector<AbstractCamera>& cameras)
		{
			int e{ -1 < userID ? eSuccess : eBadOperate };

			if (eSuccess == e)
			{
				LONG groupNo = 0;
				DWORD dwRet{ 0 };
				NET_DVR_IPPARACFG_V40 IPAccessCfgV40{ 0 };

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
								BYTE byIPID{ IPAccessCfgV40.struStreamMode[i].uGetStream.struChanInfo.byIPID };
								BYTE byIPIDHigh{ IPAccessCfgV40.struStreamMode[i].uGetStream.struChanInfo.byIPIDHigh };
								int iDevInfoIndex{ byIPIDHigh * 256 + byIPID - 1 - groupNo * 64 };

								AbstractCamera ac(IPAccessCfgV40.dwStartDChan + i);
								ac.setIPAddress(IPAccessCfgV40.struIPDevInfo[iDevInfoIndex].struIP.sIpV4);
								cameras.push_back(ac);
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
	}//namespace device
}//namespace base
