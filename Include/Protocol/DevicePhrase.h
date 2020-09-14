//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-29
//		Description :					�豸Э���װ/������
//
//		History:						Author									Date										Description
//										������									2020-05-29									����
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
			//��MessagePacketʵ��ת��ΪProtocol buffers�����л��ַ���
			//@pkt : DataPacketʵ��
			//@Return : ���л��ַ���
			const std::string packMessage(DataPacketPtr pkt);
		};//class StatusPacker
	}//namespace protocol
}//namespace base

#endif//BASE_PROTOCOL_DEVICE_PHRASE_H
