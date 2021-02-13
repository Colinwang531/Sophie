//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-29
//		Description :					��Ա������
//
//		History:						Author									Date										Description
//										������									2020-05-29									����
//

#ifndef FRAMEWORK_DATA_PROTOCOL_CREW_H
#define FRAMEWORK_DATA_PROTOCOL_CREW_H

#include <vector>
#include "Data/Data.h"

namespace framework
{
	namespace data
	{
		typedef struct tagCrewData_t
		{
			std::string id;
			std::string name;
			std::string job;
			std::vector<std::string> pictures;
		}CrewData;

		typedef struct tagCrewMsg_t
		{
			int command;
			int status;
			std::string pid;
			std::vector<CrewData> crews;
		}CrewMsg;

		class ProtocolCrew final : public Data
		{
		public:
			ProtocolCrew(void);
			~ProtocolCrew(void);

		public:
			int unpack(const std::string msg, CrewMsg& cm);
			const std::string pack(const CrewMsg& cm);
		};//class ProtocolCrew
	}//namespace data
}//namespace framework

#endif//FRAMEWORK_DATA_PROTOCOL_CREW_H
