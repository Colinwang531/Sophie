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

#ifndef COMPONENT_CODEC_H
#define COMPONENT_CODEC_H

namespace base
{
	namespace protocol
	{
		class ComponentParser
		{
		public:
			ComponentParser(void);
			~ComponentParser(void);

		public:
			//���������Ϣ��
			//@Return : ��Ϣ��ʵ��
			void* createNewComponent(void);

			//���������Ϣ��
			//@pkt : �����Ϣ��ʵ��
			//@Comment : ʹ��createNewComponent�����������Ϣ��ʵ������ʹ�ø÷�����������
			//			 ������Ҳ�������н�������
			void destroyComponent(void* pkt = nullptr);

			//���������Ϣ
			//@c : �����Ϣ����
			//@pkt : ��Ϣ��
			//@Return : ������
			int unpackComponent(
				void* c = nullptr, 
				void* pkt = nullptr);
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
			void* packComponent(
				const int command = 0,
				const int result = 0,
				void* data = nullptr);
		};//class ComponentPacker
	}//namespace protocol
}//namespace base

#endif//COMPONENT_CODEC_H
