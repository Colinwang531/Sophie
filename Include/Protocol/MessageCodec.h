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
//										������									2020-06-08									����Ϣ���Ժʹ�����з���,����ֻ��������Ϣ
//

#ifndef MESSAGE_CODEC_H
#define MESSAGE_CODEC_H

namespace base
{
	namespace protocol
	{
		class MessageParser
		{
		public:
			MessageParser(void);
			~MessageParser(void);

		public:
			//������Ϣ��
			//@Return : ��Ϣ��ʵ��
			void* createNewPacket(void);

			//������Ϣ��
			//@msg : ��Ϣ��ʵ��
			//@Comment : ʹ��createNewPacket��������Ϣ��ʵ������ʹ�ø÷�����������
			//			 ������Ҳ�������н�������
			void destroyPacket(void* pkt = nullptr);

			//��Ϣ����
			//@msg : ��Ϣ�ֽ���
			//@pkt : ��Ϣʵ��,��createNewMessage����
			//@Return : ������
			int unpackMessage(
				const char* msg = nullptr, 
				void* pkt = nullptr);

		private:
			int parseAlarmMessage(void* msg = nullptr);
			int parseAlgorithmMessage(void* msg = nullptr);
			int parseComponentMessage(void* c = nullptr, void* pkt = nullptr);
			int parseCrewMessage(void* msg = nullptr);
			int parseDeviceMessage(void* msg = nullptr);
			int parseStatusMessage(void* s = nullptr, void* pkt = nullptr);
			int parseUserMessage(void* msg = nullptr);
		};//class MessageParser

		class MessagePacker
		{
		public:
			MessagePacker(void);
			~MessagePacker(void);

		public:
			//��װ��Ϣ
			//@type : ��Ϣ����
			//@sequence : ��Ϣ���к�
			//@data : ��Ϣ����
			//@Return : ������
			const std::string packMessage(
				const int type = 0, 
				const long long sequence = 0,
				void* data = nullptr);
		};//class MessagePacker
	}//namespace protocol
}//namespace base

#endif//MESSAGE_CODEC_H
