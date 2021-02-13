//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-04-27
//		Description :					公共字段值定义
//
//		History:						Author									Date										Description
//										王科威									2020-04-27									创建
//										王科威									2020-05-13									修改命名规范
//										王科威									2020-07-02									公共数据迁移入该文件
//

#ifndef BASE_DEFINE_H
#define BASE_DEFINE_H

//端口号使用范围
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

//模型名称
static const char* gWorkerModuleName = "worker";
static const char* gClientModuleName = "client";

//组件名称
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

//命令名称
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

//CV算法参数
typedef struct tagCVParameter_t
{
	char ownID[256];				// 算法参数自身ID标识
	int gpuID;						// GPU ID标识,从0开始
	int sleepTime;					// 睡觉检测用，以秒为单位  
	float detectThreshold;			// 检测阈值  值越小越灵敏，越大误捡率越少
	float trackThreshold;			// 跟踪阈值 值越小，跟踪越久但可能出现误跟踪
	float matchThreshold;			// 人脸识别比对阈值
}CVParam;

#endif//BASE_DEFINE_H
