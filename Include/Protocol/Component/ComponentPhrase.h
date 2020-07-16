//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-15
//		Description :					���Э���װ/������
//
//		History:						Author									Date										Description
//										������									2020-05-15									����
//										������									2020-05-29									������ݽṹ�ͽ���/��װ����
//

#ifndef BASE_PROTOCOL_COMPONENT_PHRASE_H
#define BASE_PROTOCOL_COMPONENT_PHRASE_H

namespace base
{
	namespace protocol
	{
		typedef enum class tagComponentCommand_t : int
		{
			COMPONENT_COMMAND_NONE = 0,
			COMPONENT_COMMAND_SIGNIN_REQ = 1,
			COMPONENT_COMMAND_SIGNIN_REP = 2,
			COMPONENT_COMMAND_SIGNOUT_REQ = 3,
			COMPONENT_COMMAND_SIGNOUT_REP = 4,
			COMPONENT_COMMAND_QUERY_REQ = 5,
			COMPONENT_COMMAND_QUERY_REP = 6
		}ComponentCommand;

		class ComponentParser
		{
		public:
			ComponentParser(void);
			~ComponentParser(void);

		public:
			//��Protocol Buffers�����ʵ��ת��ΪAbstractPacketʵ��
			//@c : ͨ��Protocol Buffers�����õ���Componentʵ��
			//@Return : AbstractPacketʵ��
			void* unpackFromComponentMessage(void* c = nullptr);
		};//class ComponentParser

		class ComponentPacker
		{
		public:
			ComponentPacker(void);
			~ComponentPacker(void);

		public:
			//��װ�����Ϣ
			//@command : ��������
			//@result : ��COMPONENT_COMMAND_*_REP == commandʱ�ò�����Ч,��ʾӦ��״̬
			//@data : ��Ϣ����
			//		  ��COMPONENT_COMMAND_SIGNIN_REP == commandʱ��ʾ���ID��ʶ
			//		  ��COMPONENT_COMMAND_QUERY_REP == commandʱ��ʾ�����Ϣ����
			//@Return : ��Ϣ����
			void* packToComponentMessage(
				const int command = 0,
				const int result = 0,
				const void* data = nullptr);
		};//class ComponentPacker
	}//namespace protocol
}//namespace base

#endif//BASE_PROTOCOL_COMPONENT_PHRASE_H
