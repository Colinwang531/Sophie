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

#ifndef CREW_CODEC_H
#define CREW_CODEC_H

#include <vector>

namespace base
{
	namespace protocol
	{
		typedef struct tagEmployee_t
		{
			std::string name;
			std::string job;
			std::vector<std::string> pictures;
		}Employee;//struct tagEmployee_t

		typedef struct tagCrewRequest_t
		{
			Employee employee;
			std::string uid;
		}CrewRequest;//struct tagCrewRequest_t

		typedef struct tagCrewResponse_t
		{
			int result;
			std::string uid;
			std::vector<Employee> configures;
		}CrewResponse;//struct tagCrewResponse_t

		class CrewCodec
		{
		public:
			CrewCodec(void);
			virtual ~CrewCodec(void);

		public:
			int unpackCrew(void* msg = nullptr);

		private:
			int command;
			CrewRequest req;
			CrewResponse rep;
		};//class CrewCodec

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

#endif//CREW_CODEC_H
