//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-29
//		Description :					��Ϣ��Э���װ/������
//
//		History:						Author									Date										Description
//										������									2020-05-15									����
//										������									2020-06-05									�����Ϣ�����ͷ�װ��
//										������									2020-06-08									����Ϣ���Ժʹ�����з���,ֻ�ṩ������
//										������									2020-07-08									�޸������ͷ�����
//

#ifndef BASE_PROTOCOL_COMMAND_PHRASE_H
#define BASE_PROTOCOL_COMMAND_PHRASE_H

namespace base
{
	namespace protocol
	{
		class CommandParser
		{
		public:
			CommandParser(void);
			virtual ~CommandParser(void);

		public:
			//��Ϣ����
			//@data : �ֽ���
			//@databytes : �ֽ�����С
			//@Return : AbstractPacketʵ��
			void* parseFromArray(
				const void* data = nullptr, 
				const unsigned int databytes = 0);

		protected:
			//��Protocol Buffers�����ʵ��ת��ΪAbstractPacketʵ��
			//@msg : MSGʵ��
			//@Return : AbstractPacketʵ��
 			virtual void* parseAlarmMessage(void* msg = nullptr);
			virtual void* parseAlgorithmMessage(void* msg = nullptr);
			virtual void* parseComponentMessage(void* msg = nullptr);
			virtual void* parseCrewMessage(void* msg = nullptr);
			virtual void* parseDeviceMessage(void* msg = nullptr);
			virtual void* parseStatusMessage(void* msg = nullptr);
// 			int parseUserMessage(void* msg = nullptr);
		};//class CommandParser

		class CommandPacker
		{
		public:
			CommandPacker(void);
			~CommandPacker(void);

		public:
			//��װ��Ϣ
			//@type : ��Ϣ����
			//@sequence : ��Ϣ���к�
			//@data : ��Ϣ����
			//@Return : ������
			const std::string packPacketToArray(void* pkt = nullptr);
		};//class CommandPacker
	}//namespace protocol
}//namespace base

#endif//BASE_PROTOCOL_COMMAND_PHRASE_H
