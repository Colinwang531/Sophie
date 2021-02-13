//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-17
//		Description :					AIS消息数据类
//
//		History:						Author									Date										Description
//										王科威									2020-09-17									创建
//

#ifndef FRAMEWORK_DATA_AIS_DATA_H
#define FRAMEWORK_DATA_AIS_DATA_H

#include "Data/Protocol/ProtocolAIS.h"

namespace framework
{
	namespace data
	{
		class AISData : public Data
		{
		public:
			AISData(void);
			~AISData(void);

		public:
			char msgID;
			PositionA pa;
			PositionB pb;
			ShipStatic ss;
			StandardSAR sar;
			AidsToNavigation nav;
		};//class AISData
	}//namespace data
}//namespace framework

#endif//FRAMEWORK_DATA_AIS_DATA_H
