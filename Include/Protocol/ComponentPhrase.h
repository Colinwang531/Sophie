//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-15
//		Description :					���Э���װ/������
//
//		History:						Author									Date										Description
//										������									2020-05-15									����
//										������									2020-05-29									������ݽṹ�ͽ���/��װ����
//

#ifndef BASE_PROTOCOL_COMPONENT_PHRASE_H
#define BASE_PROTOCOL_COMPONENT_PHRASE_H

#include "boost/shared_ptr.hpp"
#include "Packet/DataPacket.h"
using DataPacketPtr = boost::shared_ptr<base::packet::DataPacket>;

namespace base
{
	namespace protocol
	{
		typedef enum class tagComponentCommand_t : int
		{
			COMPONENT_COMMAND_NONE = 0,
			COMPONENT_COMMAND_SIGNIN_REQ = 1,
			COMPONENT_COMMAND_SIGNIN_REP = 2,
			COMPONENT_COMMAND_SIGNOUT_REQ = 3,
			COMPONENT_COMMAND_SIGNOUT_REP = 4,
			COMPONENT_COMMAND_QUERY_REQ = 5,
			COMPONENT_COMMAND_QUERY_REP = 6
		}ComponentCommand;

		class ComponentParser
		{
		public:
			ComponentParser(void);
			~ComponentParser(void);

		public:
			//��Protocol Buffers�����ʵ��ת��ΪMessagePacketʵ��
			//@c : Protocol Buffers��װ��Componentʵ��
			//@Return : MessagePacketʵ��
			DataPacketPtr parseMessage(void* c = nullptr);
		};//class ComponentParser

		class ComponentPacker
		{
		public:
			ComponentPacker(void);
			~ComponentPacker(void);

		public:
			//��DataPacketʵ��ת��ΪProtocol buffers�����л��ַ���
			//@pkt : DataPacketʵ��
			//@Return : ���л��ַ���
			const std::string packMessage(DataPacketPtr pkt);
		};//class ComponentPacker
	}//namespace protocol
}//namespace base

#endif//BASE_PROTOCOL_COMPONENT_PHRASE_H
