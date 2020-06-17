//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-29
//		Description :					设备协议封装/解析类
//
//		History:						Author									Date										Description
//										王科威									2020-05-29									创建
//

#ifndef DEVICE_CODEC_H
#define DEVICE_CODEC_H

#include <vector>

namespace base
{
	namespace protocol
	{
		typedef enum class tagDeviceFactory_t : unsigned char
		{
			HIKVISION = 1,
			DAHUA = 2,
			EATON = 3
		}DeviceFactory;//enum class tagFactory_t

		typedef enum class tagDeviceType_t : unsigned char
		{
			DVR = 1,
			NVR = 2,
			IPC = 3
		}DeviceType;//enum class tagDeviceType_t

		typedef struct tagCamera_t
		{
			bool enable;
			int index;
			std::string cid;
			std::string nickname;
			std::string ip;
		}Camera;//struct tagCamera_t

		typedef struct tagEmbedded_t
		{
			DeviceFactory factory;
			DeviceType type;
			bool enable;
			unsigned short port;
			std::string name;
			std::string password;
			std::string ip;
			std::string did;
			std::string nickname;
			std::vector<Camera> cameras;
		}Embedded;//struct tagEmbedded_t

		typedef struct tagDeviceRequest_t
		{
			Embedded embedded;
			std::string did;
		}DeviceRequest;//struct tagDeviceRequest_t

		typedef struct tagDeviceResponse_t
		{
			int result;
			std::string did;
			std::vector<Embedded> Embeddeds;
		}DeviceResponse;//struct tagDeviceResponse_t

		class DeviceCodec
		{
		public:
			DeviceCodec(void);
			virtual ~DeviceCodec(void);

		public:
			int unpackDevice(void* msg = nullptr);

		private:
			int command;
			DeviceRequest req;
			DeviceResponse rep;
		};//class DeviceCodec
	}//namespace protocol
}//namespace base

#endif//DEVICE_CODEC_H
