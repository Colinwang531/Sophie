//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-30
//		Description :					协议库数据定义
//
//		History:						Author									Date										Description
//										王科威									 2021-06-30									 创建
//

#ifndef FRAMEWORK_LIBPROTOCOL_DEFS_H
#define FRAMEWORK_LIBPROTOCOL_DEFS_H

#include <vector>
#include <string>

namespace framework
{
    namespace libprotocol
    {
        typedef enum class tagApplicationType_t : int
        {
            APPLICATION_TYPE_NONE = 0,
            APPLICATION_TYPE_XMQ,
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
            std::string nickName;
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
    }//namespace libprotocol
}//namespace framework

#endif//FRAMEWORK_LIBPROTOCOL_DEFS_H