//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-04
//		Description :					libcommon库常量表
//
//		History:						Author									Date										Description
//										王科威									2021-06-04									创建
//

#ifndef FRAMEWORK_LIBCOMMON_CONST_DEFINE_H
#define FRAMEWORK_LIBCOMMON_CONST_DEFINE_H

//端口号
static const unsigned short gMinPortNumber = 6000;
static const unsigned short gMaxPortNumber = 65530;
static const unsigned short gDefaultXMQListenPort = 60531;
static const unsigned short gDefaultDASRecievePort = 60531;
static const unsigned short gDefaultXMSListenPort = 60820;
static const unsigned short gDefaultDHSListenPort = 60927;
static const unsigned short gDefaultAlarmPubPort = 50531;

//MTU
static const unsigned int gNetworkBufferMtuBytes = 1048576;//1M
static const unsigned int gMaxBufferSize = 3 * gNetworkBufferMtuBytes;//3M

//ģ������
static const char* gWorkerModuleName = "worker";
static const char* gClientModuleName = "client";

//�������
static const char* gUplinkComponentName = "uplink";
static const char* gXMQComponentName = "xmq";
static const char* gCMSComponentName = "cms";
static const char* gXMSComponentName = "xms";
static const char* gDHSComponentName = "dhs";
static const char* gCLKComponentName = "clk";
static const char* gALMComponentName = "alm";
static const char* gAISComponentName = "ais";
static const char* gHelmetComponentName = "helmet";
static const char* gSleepComponentName = "sleep";
static const char* gPhoneComponentName = "phone";
static const char* gFaceComponentName = "face";

//��������
static const char* gEmptyData = "";
static const char* gPairCommandName = "pair";
static const char* gComponentCommandName = "component";
static const char* gDeviceCommandName = "device";
static const char* gAlgorithmCommandName = "algorithm";
static const char* gCrewCommandName = "crew";
static const char* gStatusCommandName = "status";
static const char* gCaptureCommandName = "capture";
static const char* gAISCommandName = "ais";
static const char* gClockCommandName = "clock";
static const char* gAlarmCommandName = "alarm";
static const char* gAddressCommandName = "udp://address?";

//CV�㷨����
typedef struct tagCVParameter_t
{
	char ownID[256];				// �㷨��������ID��ʶ
	int gpuID;						// GPU ID��ʶ,��0��ʼ
	int sleepTime;					// ˯������ã�����Ϊ��λ  
	float detectThreshold;			// �����ֵ  ֵԽСԽ������Խ�������Խ��
	float trackThreshold;			// ������ֵ ֵԽС������Խ�õ����ܳ��������
	float matchThreshold;			// ����ʶ��ȶ���ֵ
}CVParam;

#endif//FRAMEWORK_LIBCOMMON_CONST_DEFINE_H
