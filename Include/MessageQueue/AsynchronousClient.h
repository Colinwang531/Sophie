//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-03
//		Description :					�첽�ͻ�����
//
//		History:						Author									Date										Description
//										������									2020-05-03									����
//

#ifndef ASYNCHRONOUS_CLIENT_H
#define ASYNCHRONOUS_CLIENT_H

namespace mq
{
	namespace module
	{
		class AsynchronousClient
		{
		public:
			AsynchronousClient(void);
			virtual ~AsynchronousClient(void);

		public:
			//�����ͻ���
			//@serverIP : �����IP��ַ
			//@serverPort : ����˶˿ں�
			//@Return : ������
			int startClient(
				const char* serverIP = nullptr,
				const unsigned short serverPort = 61001);

			//ֹͣ�ͻ���
			//@Return : ������
			int stopClient(void);

		protected:
			//�����ͻ���ͨ��ģ��
			//@serverIP : �����IP��ַ
			//@serverPort : ����˶˿ں�
			//@Return : ������
			virtual int createNewModule(
				const char* serverIP = nullptr, 
				const unsigned short serverPort = 61001);

			//���ٿͻ���ģ��
			//@Return : ������
			virtual int destroyModule(void);

			//�������ݶ�ȡ�߳�
			//@Return : ������
			virtual int startPoller(void);

			//���ݴ���ӿ�
			//@data : ���������ַ���
			//@databytes : ���������ֽ���
			//@Comment : ���ݴ����ʵ�ֱ��������������,���಻�����κ�����
			virtual void afterClientPollMessage(
				const void* data = nullptr, 
				const unsigned int databytes = 0) = 0;

		private:
			//��Ϣ�����߳�
			void pollerThreadProc(void);
			
		protected:
			void* ctx;
			void* dealer;
			//�߳��˳���ʶ
			bool stopped;
		};//class AsynchronousClient
	}//namespace module
}//namespace mq

#endif//ASYNCHRONOUS_CLIENT_H
