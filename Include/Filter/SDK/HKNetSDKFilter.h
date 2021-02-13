//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-02-06
//		Description :					海康网络SDK过滤器类
//
//		History:						Author									Date										Description
//										王科威									2021-02-06									创建
//

#ifndef FRAMEWORK_MULTIMEDIA_HK_NET_SDK_FILTER_H
#define FRAMEWORK_MULTIMEDIA_HK_NET_SDK_FILTER_H

#include "CH-HCNetSDKV6.1.4.17_build20200331_Linux64/HCNetSDK.h"
#include "Filter/Filter.h"

namespace framework
{
	namespace multimedia
	{
		class HKNetSDKFilter : public Filter
		{
		public:
			HKNetSDKFilter(void);
			virtual ~HKNetSDKFilter(void);

		public:
			//启动
			//@ipv4 : IPv4地址
			//@ipv4 : 端口号
			//@name : 用户名
			//@pwd : 密码
			//@channel : 通道索引
			//@Return : 错误码
			int start(
				const char* ipv4 = nullptr, 
				const unsigned short port = 0,
				const char* name = nullptr,
				const char* pwd = nullptr,
				const int channel = -1);

		private:
			static void CALLBACK livestreamDataCaptureCallback(
				LONG lPlayHandle, DWORD dwDataType, BYTE* pBuffer, DWORD dwBufSize, void* pUser);

		private:
			int loginID;
			int streamID;
			//数据ID标识
			//存储设备ID和通道索引号以表示数据来源
			std::string dataID;
		};//class HKNetSDKFilter
	}//namespace multimedia
}//namespace framework

#endif//FRAMEWORK_MULTIMEDIA_HK_NET_SDK_FILTER_H
