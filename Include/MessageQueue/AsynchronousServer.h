//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-04-26
//		Description :					�첽�������
//
//		History:						Author									Date										Description
//										������									2020-04-26									����
//

#ifndef ASYNCHRONOUS_SERVER_H
#define ASYNCHRONOUS_SERVER_H

#include <vector>

namespace mq
{
	namespace module
	{
		class AsynchronousServer
		{
		public:
			AsynchronousServer(void);
			virtual ~AsynchronousServer(void);

		public:
			//���������
			//@listenPort : ���ؼ����˿ں�
			//@upstreamIP : ���η����IP��ַ
			//@upstreamPort : ���η���˶˿ں�
			//@Return : ������
			//@Comment : �������Ҫ�������η����ʱ,upstreamIP��upstreamPort��2�������ǿ��Բ����õ�
			int startServer(
				const unsigned short listenPort = 61001, 
				const char* upstreamIP = nullptr, 
				const unsigned short upstreamPort = 61101);

			//ֹͣ�����
			//@Return : ������
 			int stopServer(void);

		protected:
			//�������ط����ͨ��ģ��
			//@listenPort : ���ؼ����˿ں�
			//@Return : ������
			//@Comment : listenPort���������ڸ����ж���,���ڱ��ؼ����˿�
			//			 �ڶ����͵����������������ඨ��,�����������ӵ�ַ�Ͷ˿�
			virtual int createNewModule(
				const unsigned short listenPort = 61001, 
				const char* = nullptr, 
				const unsigned short = 0);

			//���ٷ����ģ��
			//@Return : ������
			virtual int destroyModule(void);

			//�������ݶ�ȡ�߳�
			//@Return : ������
			virtual int startPoller(void);

			//���ݴ���ӿ�
			//@s : SOCKET��ʶ
			//@id : ID��ʶ�ַ���
			//@idbytes : ID��ʶ�ֽ���
			//@delimiter : �ָ����ַ���
			//@delimiterbytes : �ָ����ֽ���
			//@data : ���������ַ���
			//@databytes : ���������ֽ���
			//@Comment : ���ݴ����ʵ�ֱ��������������,���಻�����κ�����
			virtual void afterServerPollMessage(
				void* s = nullptr,
				const char* id = nullptr, 
				const unsigned int idbytes = 0,
				const char* delimiter = nullptr, 
				const unsigned int delimiterbytes = 0,
				const char* data = nullptr, 
				const unsigned int databytes = 0) = 0;

		private:
			//��Ϣת���߳�
			void pollerThreadProc(void);

			//�����첽����˴����߳�
			//@Return : ������
			int startWorker(void);

			//��Ϣ�����߳�
			void workerThreadProc(void);

			//��Ϣ�ַ�����
			//@src : ����ԴID
			//@dest : Ŀ��ID
			void transferData(void* src = nullptr, void* dest = nullptr);

		protected:
			void* ctx;
			//�߳��˳���ʶ
			bool stopped;

		private:
			void* router;
			void* dealer;
		};//class AsynchronousServer
	}//namespace module
}//namespace mq

#endif//ASYNCHRONOUS_SERVER_H
