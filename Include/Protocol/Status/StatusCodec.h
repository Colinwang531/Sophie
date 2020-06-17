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

#ifndef STATUS_CODEC_H
#define STATUS_CODEC_H

namespace base
{
	namespace protocol
	{
		class StatusParser
		{
		public:
			StatusParser(void);
			~StatusParser(void);

		public:
			//����״̬��Ϣ��
			//@Return : ��Ϣ��ʵ��
			void* createNewStatus(void);

			//����״̬��Ϣ��
			//@pkt : ״̬��Ϣ��ʵ��
			//@Comment : ʹ��createNewStatus������״̬��Ϣ��ʵ������ʹ�ø÷�����������
			//			 ������Ҳ�������н�������
			void destroyStatus(void* pkt = nullptr);

			//����״̬��Ϣ
			//@c : ״̬��Ϣ����
			//@pkt : ��Ϣ��
			//@Return : ������
			int unpackStatus(
				void* s = nullptr, 
				void* pkt = nullptr);
		};//class StatusParser
	}//namespace protocol
}//namespace base

#endif//STATUS_CODEC_H
