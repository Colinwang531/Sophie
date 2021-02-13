//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-04
//		Description :					状态协议封装/解析类
//
//		History:						Author									Date										Description
//										王科威									2020-10-04									创建
//

#ifndef FRAMEWORK_DATA_PROTOCOL_CLOCK_H
#define FRAMEWORK_DATA_PROTOCOL_CLOCK_H

#include "Data/Data.h"

namespace framework
{
	namespace data
	{
		class ProtocolClock final : public Data
		{
		public:
			ProtocolClock(void);
			~ProtocolClock(void);

		public:
			const std::string unpack(const std::string msg);
			const std::string pack(const std::string str);
		};//class ProtocolClock
	}//namespace data
}//namespace framework

#endif//FRAMEWORK_DATA_PROTOCOL_CLOCK_H
