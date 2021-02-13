//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-17
//		Description :					AIS��Ϣ������
//
//		History:						Author									Date										Description
//										������									2020-09-17									����
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
