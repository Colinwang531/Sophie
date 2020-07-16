//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-29
//		Description :					״̬Э���װ/������
//
//		History:						Author									Date										Description
//										������									2020-05-29									����
//										������									2020-06-12									������ݽṹ�ͽ���/��װ����
//

#ifndef BASE_PROTOCOL_STATUS_PHRASE_H
#define BASE_PROTOCOL_STATUS_PHRASE_H

namespace base
{
	namespace protocol
	{
		typedef enum class tagStatusCommand_t : int
		{
			STATUS_COMMAND_NONE = 0,
			STATUS_COMMAND_SET_REQ = 1,
			STATUS_COMMAND_SET_REP = 2,
			STATUS_COMMAND_QUERY_REQ = 3,
			STATUS_COMMAND_QUERY_REP = 3,
		}StatusCommand;

		typedef enum class tagStatusType_t : int
		{
			STATUS_TYPE_NONE = 0,
			STATUS_TYPE_SAIL = 1,
			STATUS_TYPE_AUTO = 2,
			STATUS_TYPE_NAME = 3
		}StatusType;

		class StatusParser
		{
		public:
			StatusParser(void);
			~StatusParser(void);

		public:
			//��Protocol Buffers��״̬ʵ��ת��ΪAbstractPacketʵ��
			//@s : ͨ��Protocol Buffers�����õ���Statusʵ��
			//@Return : AbstractPacketʵ��
			void* unpackFromStatusMessage(void* s = nullptr);
		};//class StatusParser

		class StatusPacker
		{
		public:
			StatusPacker(void);
			~StatusPacker(void);

		public:
			//��װ״̬��Ϣ
			//@command : ��������
			//@result : ��COMPONENT_COMMAND_*_REP == commandʱ�ò�����Ч,��ʾӦ��״̬
			//@data : ��Ϣ����
			//		  ��COMPONENT_COMMAND_SIGNIN_REP == commandʱ��ʾ���ID��ʶ
			//		  ��COMPONENT_COMMAND_QUERY_REP == commandʱ��ʾ�����Ϣ����
			//@Return : ��Ϣ����
			void* packStatus(
				const int command = 0,
				const int result = 0,
				void* data = nullptr);
		};//class StatusPacker
	}//namespace protocol
}//namespace base

#endif//BASE_PROTOCOL_STATUS_PHRASE_H
