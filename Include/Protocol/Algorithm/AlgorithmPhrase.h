//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-29
//		Description :					�㷨Э���װ/������
//
//		History:						Author									Date										Description
//										������									2020-05-29									����
//

#ifndef BASE_PROTOCOL_ALGORITHM_PHRASE_H
#define BASE_PROTOCOL_ALGORITHM_PHRASE_H

namespace base
{
	namespace protocol
	{
		typedef enum class tagAlgorithmCommand_t : int
		{
			ALGORITHM_COMMAND_NONE = 0,
			ALGORITHM_COMMAND_SET_REQ = 1,
			ALGORITHM_COMMAND_SET_REP = 2,
			ALGORITHM_COMMAND_QUERY_REQ = 3,
			ALGORITHM_COMMAND_QUERY_REP = 3,
		}AlgorithmCommand;

		class AlgorithmParser
		{
		public:
			AlgorithmParser(void);
			~AlgorithmParser(void);

		public:
			void* parseAlgorithmMessage(void* a = nullptr);
		};//class AlgorithmParser

		class AlgorithmPacker
		{
		public:
			AlgorithmPacker(void);
			~AlgorithmPacker(void);

		public:
			//��װ�㷨��Ϣ
			//@command : ��������
			//@result : ������
			//@data : ��Ϣ����
			//@Return : ��Ϣ����
			void* packAlgorithm(
				const int command = 0,
				const int result = 0,
				void* data = nullptr);
		};//class AlgorithmPacker
	}//namespace protocol
}//namespace base

#endif//BASE_PROTOCOL_ALGORITHM_PHRASE_H
