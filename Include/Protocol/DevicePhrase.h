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

#ifndef BASE_PROTOCOL_DEVICE_PHRASE_H
#define BASE_PROTOCOL_DEVICE_PHRASE_H

#include "boost/shared_ptr.hpp"
#include "Packet/DataPacket.h"
using DataPacketPtr = boost::shared_ptr<base::packet::DataPacket>;

namespace base
{
	namespace protocol
	{
		typedef enum class tagDeviceCommand_t : int
		{
			DEVICE_COMMAND_NONE = 0,
			DEVICE_COMMAND_NEW_REQ = 1,
			DEVICE_COMMAND_NEW_REP = 2,
			DEVICE_COMMAND_DELETE_REQ = 3,
			DEVICE_COMMAND_DELETE_REP = 4,
			DEVICE_COMMAND_QUERY_REQ = 5,
			DEVICE_COMMAND_QUERY_REP = 6,
			DEVICE_COMMAND_MODIFY_REQ = 5,
			DEVICE_COMMAND_MODIFY_REP = 6
		}DeviceCommand;

		class DeviceParser
		{
		public:
			DeviceParser(void);
			~DeviceParser(void);

		public:
			DataPacketPtr parseMessage(void* d = nullptr);
		};//class DeviceParser

		class DevicePacker
		{
		public:
			DevicePacker(void);
			~DevicePacker(void);

		public:
			//将MessagePacket实例转换为Protocol buffers的序列化字符串
			//@pkt : DataPacket实例
			//@Return : 序列化字符串
			const std::string packMessage(DataPacketPtr pkt);
		};//class StatusPacker
	}//namespace protocol
}//namespace base

#endif//BASE_PROTOCOL_DEVICE_PHRASE_H
