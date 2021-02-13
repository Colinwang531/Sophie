//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-29
//		Description :					设备数据类
//
//		History:						Author									Date										Description
//										王科威									2020-05-29									创建
//

#ifndef FRAMEWORK_DATA_PROTOCOL_DEVICE_H
#define FRAMEWORK_DATA_PROTOCOL_DEVICE_H

#include <vector>
#include "Data/Data.h"

namespace framework
{
	namespace data
	{
		typedef struct tagCameraData_t
		{
			bool enable;
			int type;
			int index;
			std::string did;
			std::string ipv4;
			std::string nickname;
		}CameraData;

		typedef struct tagDeviceData_t
		{
			bool enable;
			unsigned short port;
			int factory;
			int type;
			std::string did;
			std::string username;
			std::string userpwd;
			std::string ipv4;
			std::string nickname;
			std::vector<CameraData> cds;
		}DeviceData;

		typedef struct tagDeviceMsg_t
		{
			int command;
			int status;
			std::string pid;
			std::vector<DeviceData> dds;
		}DeviceMsg;

		class ProtocolDevice final : public Data
		{
		public:
			ProtocolDevice(void);
			~ProtocolDevice(void);

		public:
			int unpack(const std::string msg, DeviceMsg& dm);
			const std::string pack(const DeviceMsg& dm);
		};//class StatusPacker
	}//namespace protocol
}//namespace base

#endif//FRAMEWORK_DATA_PROTOCOL_DEVICE_H
