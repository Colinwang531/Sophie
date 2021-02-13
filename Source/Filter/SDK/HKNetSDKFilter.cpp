#include "boost/format.hpp"
#include "boost/make_shared.hpp"
#include "Define.h"
#include "Error.h"
#include "Pin/Pin.h"
#include "Filter/SDK/HKNetSDKFilter.h"

namespace framework
{
	namespace multimedia
	{
		HKNetSDKFilter::HKNetSDKFilter()
			: Filter(FilterType::FILTER_TYPE_SOURCE), loginID{ -1 }, streamID{ -1 }
		{
			Filter::addPin(OutputPinName);
		}
		HKNetSDKFilter::~HKNetSDKFilter()
		{}

		int HKNetSDKFilter::start(
			const char* ipv4 /* = nullptr */, 
			const unsigned short port /* = 0 */, 
			const char* name /* = nullptr */, 
			const char* pwd /* = nullptr */, 
			const int channel /* = -1 */)
		{
			int e{ 
				ipv4 && gMaxPortNumber > port && gMinPortNumber < port && 
				name && pwd && -1 < channel ? eSuccess : eInvalidParameter };
			
			if (eSuccess == e)
			{
				if (NET_DVR_Init())
				{
					NET_DVR_USER_LOGIN_INFO loginInfo{ 0 };
					loginInfo.bUseAsynLogin = 0;
					strcpy(loginInfo.sDeviceAddress, ipv4);
					loginInfo.wPort = port;
					strcpy(loginInfo.sUserName, name);
					strcpy(loginInfo.sPassword, pwd);
					NET_DVR_DEVICEINFO_V40 deviceInfo{ 0 };

					//Í¬²½µÇÂ¼
					loginID = NET_DVR_Login_V40(&loginInfo, &deviceInfo);
					if (-1 < loginID)
					{
						NET_DVR_PREVIEWINFO previewInfo{ 0 };
						previewInfo.lChannel = channel;
						streamID = NET_DVR_RealPlay_V40(
							loginID, &previewInfo, &HKNetSDKFilter::livestreamDataCaptureCallback, this);
					}
					else
					{
						e = eBadLoginDevice;
					}
				}
				else
				{
					e = eBadInitSDK;
				}
			}

			return e;
		}

		void HKNetSDKFilter::livestreamDataCaptureCallback(
			LONG lPlayHandle, DWORD dwDataType, BYTE* pBuffer, DWORD dwBufSize, void* pUser)
		{
			HKNetSDKFilter* filter{ reinterpret_cast<HKNetSDKFilter*>(pUser) };

			if (lPlayHandle == filter->streamID)
			{
				if (NET_DVR_SYSHEAD == dwDataType || NET_DVR_STREAMDATA == dwDataType)
				{
					DataPtr dp{ 
						boost::make_shared<Data>(
							framework::data::DataFactory::DATA_FACTORY_HK,
							NET_DVR_SYSHEAD == dwDataType ? framework::data::DataType::DATA_TYPE_HEADER : framework::data::DataType::DATA_TYPE_BYTES)};

					if (eSuccess == dp->setData(pBuffer, static_cast<int>(dwBufSize)))
					{
						filter->inputData(dp);
					}
				}
			}
		}
	}//namespace multimedia
}//namespace framework
