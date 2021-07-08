//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-04
//		Description :					数据定义
//
//		History:						Author									Date										Description
//										王科威									2021-06-04									创建
//

#ifndef FRAMEWORK_LIBCOMMON_DEFS_H
#define FRAMEWORK_LIBCOMMON_DEFS_H

#include <string>
#include <vector>

//端口号
static const unsigned short gMinPortNumber = 6000;
static const unsigned short gMaxPortNumber = 65530;

//MTU
static const unsigned int gNetworkBufferMtuBytes = 1048576;//1M
static const unsigned int gMaxBufferSize = 3 * gNetworkBufferMtuBytes;//3M

typedef enum class tagAISDataType_t : int
{
	AIS_DATA_TYPE_NONE = 0,
	AIS_DATA_TYPE_POSITION_A,
	AIS_DATA_TYPE_POSITION_B,
	AIS_DATA_TYPE_SHIP_STATIC,
	AIS_DATA_TYPE_STANDARD_SAR,
	AIS_DATA_TYPE_AIDS_TO_NAVIGATION
}AISDataType;

typedef enum class tagRegisterQueryType_t : int
{
	REGISTER_QUERY_TYPE_NONE = 0,
	REGISTER_QUERY_TYPE_REGISTER_REQ,
	REGISTER_QUERY_TYPE_REGISTER_REP,
	REGISTER_QUERY_TYPE_QUERY_REQ,
	REGISTER_QUERY_TYPE_QUERY_REP
}RegisterQueryType;

typedef enum class tagNotifyType_t : int
{
	NOTIFY_TYPE_NONE = 0,
	NOTIFY_TYPE_AIS_POSITION_A,
	NOTIFY_TYPE_AIS_POSITION_B,
	NOTIFY_TYPE_AIS_SHIP_STATIC,
	NOTIFY_TYPE_AIS_STANDARD_SAR,
	NOTIFY_TYPE_AIS_AIDS_TO_NAVIGATION,
	NOTIFY_TYPE_CLOCK_SYC
}NotifyType;

typedef enum class tagApplicationType_t : int
{
	APPLICATION_TYPE_NONE = 0,
	APPLICATION_TYPE_XMQ,
	APPLICATION_TYPE_CMS,
	APPLICATION_TYPE_WEB,
	APPLICATION_TYPE_DHSLB,
	APPLICATION_TYPE_DHS,
	APPLICATION_TYPE_XMSLB,
	APPLICATION_TYPE_XMS,
	APPLICATION_TYPE_RTSP,
	APPLICATION_TYPE_RTMP,
	APPLICATION_TYPE_HLS,
	APPLICATION_TYPE_GBS,
	APPLICATION_TYPE_BGXMS,
	APPLICATION_TYPE_DSS,
	APPLICATION_TYPE_DBE,
	APPLICATION_TYPE_AIS,
	APPLICATION_TYPE_TSS,
	APPLICATION_TYPE_ALARM,
	APPLICATION_TYPE_HELMET,
	APPLICATION_TYPE_PHONE,
	APPLICATION_TYPE_SLEEP,
	APPLICATION_TYPE_FACE,
	APPLICATION_TYPE_FIGHT,
	APPLICATION_TYPE_FIRE
}ApplicationType;

typedef struct tagApplicationInfo_t
{
	ApplicationType type;
	std::string name;
	std::string IPv4;
	std::string ID;
	std::string parentID;
	std::string nickName;
	unsigned long long timestamp;
}ApplicationInfo;

typedef enum class tagAlgorithmType_t : int
{
	ALGORITHM_TYPE_NONE = 0,
	ALGORITHM_TYPE_HELMET,
	ALGORITHM_TYPE_PHONE,
	ALGORITHM_TYPE_SLEEP,
	ALGORITHM_TYPE_FACE,
	ALGORITHM_TYPE_FIRE
}AlgorithmType;

typedef struct tagAlgorithmInfo_t
{
	AlgorithmType type;
	bool checkin;
	int gpuIndex;
	int triggerInterval;
	float detectThreshold;
	float trackThreshold;
	float secondDetectThreshold;
	float similarity;
	std::string ID;
	std::string cameraID;
}AlgorithmInfo;

typedef enum class tagCameraType_t : int
{
	CAMERA_TYPE_NONE = 0,
	CAMERA_TYPE_ANALOG,
	CAMERA_TYPE_DIGIST
}CameraType;

typedef struct tagCameraInfo_t
{
	CameraType type;
	bool enable;
	int cameraIndex;
	std::string ID;
	std::string nickName;
	std::string IPv4;
}CameraInfo;

typedef enum class tagFactoryType_t : int
{
	FACTORY_TYPE_NONE = 0,
	FACTORY_TYPE_HK,
	FACTORY_TYPE_DH,
	FACTORY_TYPE_ET
}FactoryType;

typedef enum class tagDeviceMode_t : int
{
	DEVICE_MODE_NONE = 0,
	DEVICE_MODE_DVR,
	DEVICE_MODE_NVR,
	DEVICE_MODE_IPC
}DeviceMode;

typedef struct tagDeviceInfo_t
{
	FactoryType factory;
	DeviceMode mode;
	bool enable;
	int port;
	std::string ID;
	std::string userName;
	std::string userPassword;
	std::string IPv4;
	std::string nickName;
	std::vector<CameraInfo> infos;
}DeviceInfo;

typedef struct tagCrewInfo_t
{
	std::string ID;
	std::string name;
	std::string job;
	std::vector<std::string> pictures;
}CrewInfo;

typedef struct tagPositionA_t
{
	int repeatindicator;
	int navigationalstatus;
	float rot;
	float sog;
	int positionaccuracy;
	float cog;
	int trueheading;
	int timestamp;
	int specialindicator;
	int spare;
	std::string longitude;
	std::string latitude;
	std::string mmsi;
}PositionA;

typedef struct tagPositionB_t
{
	int repeatindicator;
	int spare1;
	float sog;
	int positionaccuracy;
	float cog;
	int trueheading;
	int timestamp;
	int spare2;
	int bunitflag;
	int bdisplayflag;
	int bdscflag;
	int bbandflag;
	int bmessage22flag;
	int modeflag;
	int raimflag;
	int commstateselectorflag;
	int syncstate;
	int slotincrement;
	int numberofslots;
	int keepflag;
	std::string longitude;
	std::string latitude;
	std::string mmsi;
}PositionB;

typedef struct tagShipStatic_t
{
	int repeatindicator;
	int aisversion;
	int imo;
	int typeofshipcargo;
	int epfd;
	int month;
	int day;
	int hour;
	int minute;
	int dte;
	int spare;
	float draught;
	std::string mmsi;
	std::string callsign;
	std::string name;
	std::string dimensions;
	std::string destination;
}ShipStatic;

typedef struct tagStandardSAR_t
{
	int repeatindicator;
	int altitude;
	int positionaccuracy;
	int timestamp;
	int altitudesensor;
	int spare;
	int assignmodeflag;
	int raimflag;
	int commstateselectorflag;
	int communicationstate;
	float sog;
	float cog;
	std::string mmsi;
	std::string longitude;
	std::string latitude;
}StandardSAR;

typedef struct tagAidsToNavigation_t
{
	int repeatindicator;
	int navigationtype;
	int positionaccuracy;
	int epfd;
	int utctimestamp;
	int onoffpositionindicator;
	int atonflag;
	int raimflag;
	int virtualflag;
	int modeindicator;
	int spare;
	int stufbits;
	std::string mmsi;
	std::string name;
	std::string longitude;
	std::string latitude;
	std::string dimensions;
	std::string extendname;
}AidsToNavigation;

typedef struct tagPositionInfo_t
{
	int x;
	int y;
	int w;
	int h;
}PositionInfo;

typedef enum tagAlarmType_t
{
	ALARM_TYPE_NONE = 0,
	ALARM_TYPE_HELMET,
	ALARM_TYPE_PHONE,
	ALARM_TYPE_SLEEP,
	ALARM_TYPE_FIGHT,
	ALARM_TYPE_FACE,
	ALARM_TYPE_BODY,
	ALARM_TYPE_FIRE
}AlarmType;

typedef struct tagAlarmInfo_t
{
	AlarmType type;
	bool checkin;
	int personNum;
	std::string cameraID;
	std::string timestamp;
	std::string picture;
	std::vector<PositionInfo> positions;
	std::string faceID;
}AlarmInfo;

#endif//FRAMEWORK_LIBCOMMON_DEFS_H
