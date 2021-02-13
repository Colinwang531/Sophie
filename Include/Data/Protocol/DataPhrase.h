//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-08-28
//		Description :					数据封装/解析类
//
//		History:						Author									Date										Description
//										王科威									2020-08-28									创建
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
			//数据解析
			//@data : 数据内容
			//@Return : DataPacket实例
			DataPacketPtr parseData(const std::string data);
		};//class DataParser

		class DataPacker
		{
		public:
			DataPacker(void);
			virtual ~DataPacker(void);

		public:
			//数据封装
			//@pkt : DataPacket实例
			//@Return : 数据内容
			const std::string packData(DataPacketPtr pkt);
		};//class DataPacker
	}//namespace protocol
}//namespace base

#endif//BASE_PROTOCOL_DATA_PHRASE_H
