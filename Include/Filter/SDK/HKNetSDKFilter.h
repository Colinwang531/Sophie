//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-02-06
//		Description :					��������SDK��������
//
//		History:						Author									Date										Description
//										������									2021-02-06									����
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
			//����
			//@ipv4 : IPv4��ַ
			//@ipv4 : �˿ں�
			//@name : �û���
			//@pwd : ����
			//@channel : ͨ������
			//@Return : ������
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
			//����ID��ʶ
			//�洢�豸ID��ͨ���������Ա�ʾ������Դ
			std::string dataID;
		};//class HKNetSDKFilter
	}//namespace multimedia
}//namespace framework

#endif//FRAMEWORK_MULTIMEDIA_HK_NET_SDK_FILTER_H
