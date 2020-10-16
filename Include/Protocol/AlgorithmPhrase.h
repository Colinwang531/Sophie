//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-29
//		Description :					�㷨Э���װ/������
//
//		History:						Author									Date										Description
//										������									2020-05-29									����
//

#ifndef BASE_PROTOCOL_ALGORITHM_PHRASE_H
#define BASE_PROTOCOL_ALGORITHM_PHRASE_H

#include "boost/shared_ptr.hpp"
#include "Packet/DataPacket.h"
using DataPacketPtr = boost::shared_ptr<base::packet::DataPacket>;

namespace base
{
	namespace protocol
	{
		typedef enum class tagAlgorithmCommand_t : int
		{
			ALGORITHM_COMMAND_NONE = 0,
			ALGORITHM_COMMAND_CONFIG_REQ = 1,
			ALGORITHM_COMMAND_CONFIG_REP = 2,
			ALGORITHM_COMMAND_QUERY_REQ = 3,
			ALGORITHM_COMMAND_QUERY_REP = 3,
		}AlgorithmCommand;

		class AlgorithmParser
		{
		public:
			AlgorithmParser(void);
			~AlgorithmParser(void);

		public:
			DataPacketPtr parseMessage(void* a = nullptr);
		};//class AlgorithmParser

		class AlgorithmPacker
		{
		public:
			AlgorithmPacker(void);
			~AlgorithmPacker(void);

		public:
			//��װ�㷨��Ϣ
			//@command : ��������
			//@result : ������
			//@data : ��Ϣ����
			//@Return : ��Ϣ����
			const std::string packMessage(DataPacketPtr pkt);
		};//class AlgorithmPacker
	}//namespace protocol
}//namespace base

#endif//BASE_PROTOCOL_ALGORITHM_PHRASE_H
