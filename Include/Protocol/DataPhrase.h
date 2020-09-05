//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-08-28
//		Description :					���ݷ�װ/������
//
//		History:						Author									Date										Description
//										������									2020-08-28									����
//

#ifndef BASE_PROTOCOL_DATA_PHRASE_H
#define BASE_PROTOCOL_DATA_PHRASE_H

#include "boost/shared_ptr.hpp"
#include "Packet/DataPacket.h"
using DataPacketPtr = boost::shared_ptr<base::packet::DataPacket>;

namespace base
{
	namespace protocol
	{
		class DataParser
		{
		public:
			DataParser(void);
			virtual ~DataParser(void);

		public:
			//���ݽ���
			//@data : ��������
			//@Return : DataPacketʵ��
			DataPacketPtr parseData(const std::string data);
		};//class DataParser

		class DataPacker
		{
		public:
			DataPacker(void);
			virtual ~DataPacker(void);

		public:
			//���ݷ�װ
			//@pkt : DataPacketʵ��
			//@Return : ��������
			const std::string packData(DataPacketPtr pkt);
		};//class DataPacker
	}//namespace protocol
}//namespace base

#endif//BASE_PROTOCOL_DATA_PHRASE_H
