//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-04
//		Description :					״̬Э���װ/������
//
//		History:						Author									Date										Description
//										������									2020-10-04									����
//

#ifndef BASE_PROTOCOL_EVENT_PHRASE_H
#define BASE_PROTOCOL_EVENT_PHRASE_H

#include "boost/shared_ptr.hpp"
#include "Packet/DataPacket.h"
using DataPacketPtr = boost::shared_ptr<base::packet::DataPacket>;

namespace base
{
	namespace protocol
	{
		typedef enum class tagEventCommand_t : int
		{
			EVENT_COMMAND_NONE = 0,
			EVENT_COMMAND_CAPTURE_JPEG_REQ = 1,
			EVENT_COMMAND_CAPTURE_JPEG_REP = 2,
		}EventCommand;

		class EventParser
		{
		public:
			EventParser(void);
			~EventParser(void);

		public:
			//��Protocol Buffers�����ʵ��ת��ΪMessagePacketʵ��
			//@e : Protocol Buffers��װ��Eventʵ��
			//@Return : MessagePacketʵ��
			DataPacketPtr parseMessage(void* e = nullptr);
		};//class EventParser

		class EventPacker
		{
		public:
			EventPacker(void);
			~EventPacker(void);

		public:
			const std::string packMessage(DataPacketPtr pkt);
		};//class EventPacker
	}//namespace protocol
}//namespace base

#endif//BASE_PROTOCOL_EVENT_PHRASE_H
