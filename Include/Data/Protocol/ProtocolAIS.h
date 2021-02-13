//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-01-22
//		Description :					AIS数据类
//
//		History:						Author									Date										Description
//										王科威									2021-01-22									创建
//

#ifndef FRAMEWORK_DATA_PROTOCOL_AIS_H
#define FRAMEWORK_DATA_PROTOCOL_AIS_H

#include <string>
#include "Data/Data.h"

namespace framework
{
	namespace data
	{
		typedef struct tagPositionA_t
		{
			int repeatindicator;
			int navigationalstatus;
			float rot;
			float sog;
			int positionaccuracy;
			float cog;
			int trueheading;
			int timestamp;
			int specialindicator;
			int spare;
			std::string longitude;
			std::string latitude;
			std::string mmsi;
		}PositionA;

		typedef struct tagPositionB_t
		{
			int repeatindicator;
			int spare1;
			float sog;
			int positionaccuracy;
			float cog;
			int trueheading;
			int timestamp;
			int spare2;
			int bunitflag;
			int bdisplayflag;
			int bdscflag;
			int bbandflag;
			int bmessage22flag;
			int modeflag;
			int raimflag;
			int commstateselectorflag;
			int syncstate;
			int slotincrement;
			int numberofslots;
			int keepflag;
			std::string longitude;
			std::string latitude;
			std::string mmsi;
		}PositionB;

		typedef struct tagShipStatic_t
		{
			int repeatindicator;
			int aisversion;
			int imo;
			int typeofshipcargo;
			int epfd;
			int month;
			int day;
			int hour;
			int minute;
			int dte;
			int spare;
			float draught;
			std::string mmsi;
			std::string callsign;
			std::string name;
			std::string dimensions;
			std::string destination;
		}ShipStatic;

		typedef struct tagStandardSAR_t
		{
			int repeatindicator;
			int altitude;
			int positionaccuracy;
			int timestamp;
			int altitudesensor;
			int spare;
			int assignmodeflag;
			int raimflag;
			int commstateselectorflag;
			int communicationstate;
			float sog;
			float cog;
			std::string mmsi;
			std::string longitude;
			std::string latitude;
		}StandardSAR;

		typedef struct tagAidsToNavigation_t
		{
			int repeatindicator;
			int navigationtype;
			int positionaccuracy;
			int epfd;
			int utctimestamp;
			int onoffpositionindicator;
			int atonflag;
			int raimflag;
			int virtualflag;
			int modeindicator;
			int spare;
			int stufbits;
			std::string mmsi;
			std::string name;
			std::string longitude;
			std::string latitude;
			std::string dimensions;
			std::string extendname;
		}AidsToNavigation;

		typedef struct tagAISMsg_t
		{
			int type;
			std::string pid;
			PositionA positiona;
			PositionB positionb;
			ShipStatic shipstatic;
			StandardSAR standardsar;
			AidsToNavigation aidstonavigation;
		}AISMsg;

		class ProtocolAIS final : public Data
		{
		public:
			ProtocolAIS(void);
			~ProtocolAIS(void);

		public:
			int unpack(const std::string msg, AISMsg& am);
			const std::string pack(const AISMsg& am);
		};//class ProtocolAIS
	}//namespace data
}//namespace framework

#endif//FRAMEWORK_DATA_PROTOCOL_AIS_H
