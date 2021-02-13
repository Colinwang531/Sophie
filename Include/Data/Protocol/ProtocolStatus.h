//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-01-22
//		Description :					״̬������
//
//		History:						Author									Date										Description
//										������									2021-01-22									����
//

#ifndef FRAMEWORK_DATA_PROTOCOL_STATUS_H
#define FRAMEWORK_DATA_PROTOCOL_STATUS_H

#include <string.h>
#include "Data/Data.h"

namespace framework
{
	namespace data
	{
		typedef struct tagManualController_t
		{
			bool flag;
			bool state;
		}ManualController;

		typedef struct tagStatusMsg_t
		{
			int command;
			int state;
			int status;
			ManualController mc;
			std::string pid;
		}StatusMsg;

		class ProtocolStatus final : public Data
		{
		public:
			ProtocolStatus(void);
			~ProtocolStatus(void);

		public:
			int unpack(const std::string msg, StatusMsg& sm);
			const std::string pack(const StatusMsg& sm);
		};//class ProtocolStatus
	}//namespace data
}//namespace framework

#endif//FRAMEWORK_DATA_PROTOCOL_STATUS_H
