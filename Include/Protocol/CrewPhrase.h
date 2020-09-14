//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-29
//		Description :					��ԱЭ���װ/������
//
//		History:						Author									Date										Description
//										������									2020-05-29									����
//

#ifndef BASE_PROTOCOL_CREW_PHRASE_H
#define BASE_PROTOCOL_CREW_PHRASE_H

namespace base
{
	namespace protocol
	{
		typedef enum class tagCrewCommand_t : int
		{
			CREW_COMMAND_NONE = 0,
			CREW_COMMAND_NEW_REQ = 1,
			CREW_COMMAND_NEW_REP = 2,
			CREW_COMMAND_DELETE_REQ = 3,
			CREW_COMMAND_DELETE_REP = 4,
			CREW_COMMAND_MODIFY_REQ = 5,
			CREW_COMMAND_MODIFY_REP = 6,
			CREW_COMMAND_QUERY_REQ = 7,
			CREW_COMMAND_QUERY_REP = 8
		}CrewCommand;

		class CrewParser
		{
		public:
			CrewParser(void);
			~CrewParser(void);

		public:
			void* parseCrewMessage(void* c = nullptr);
		};//class CrewParser

		class CrewPacker
		{
		public:
			CrewPacker(void);
			~CrewPacker(void);

		public:
			//��װ��Ա��Ϣ
			//@command : ��������
			//@result : ������
			//@data : ��Ϣ����
			//@Return : ��Ϣ����
			void* packCrew(
				const int command = 0,
				const int result = 0,
				void* data = nullptr);
		};//class CrewPacker
	}//namespace protocol
}//namespace base

#endif//BASE_PROTOCOL_CREW_PHRASE_H
