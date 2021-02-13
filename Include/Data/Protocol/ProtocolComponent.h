//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-15
//		Description :					���������
//
//		History:						Author									Date										Description
//										������									2020-05-15									����
//										������									2021-01-18									�ع����������
//

#ifndef FRAMEWORK_DATA_PROTOCOL_COMPONENT_H
#define FRAMEWORK_DATA_PROTOCOL_COMPONENT_H

#include <vector>
#include "Data/Data.h"

namespace framework
{
	namespace data
	{
		typedef struct tagComponentData_t
		{
			int type;
			std::string name;
			std::string ipv4;
			std::string id;
			std::string nickname;
		}ComponentData;

		typedef struct tagComponentMsg_t
		{
			int command;
			int status;
			std::string pid;
			std::vector<ComponentData> cds;
		}ComponentMsg;
		
		class ProtocolComponent final : public Data
		{
		public:
			ProtocolComponent(void);
			~ProtocolComponent(void);

		public:
			int unpack(const std::string msg, ComponentMsg& cm);
			const std::string pack(const ComponentMsg& cm);
		};//class ProtocolComponent
	}//namespace data
}//namespace framework

#endif//FRAMEWORK_DATA_PROTOCOL_COMPONENT_H
