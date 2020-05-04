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
			//@port : ����˼����˿ں�
			//@upstreamAddress : ���η���˵�ַ
			//@upstreamPort : ���η���˶˿ں�
			//@Return : ������ֵ
			int startServer(
				const unsigned short port = 61001, const char* upstreamAddress = nullptr, const unsigned short upstreamPort = 61101);

			//ֹͣ�����
			//@Return : ������ֵ
 			int stopServer(void);

		protected:
			//���������ģ��
			//�̳����ڸ÷�����ʵ���Զ���ģ�͵Ĵ���
			//@port : ����˼����˿ں�
			//@Return : ������ֵ
			virtual int createNewModule(const unsigned short port = 61001, const char* address = nullptr);

			//���ٷ����ģ��
			//@Return : ������ֵ
			virtual int destroyModule(void);

			//������ݶ�ȡʵ����
			//@items : ���ݶ�ȡʵ������
			virtual void addPollItem(std::vector<void*>& items);

			//���ݶ�ȡ����
			//@s : ���ݶ�ȡ��socketʵ��
			virtual void afterPollItemMessage(void* s = nullptr);

		private:
			//�����첽��������ݶ�ȡ
			//@Return : ������ֵ
			int startPoller(void);
			static void pollerThreadProc(void* param = nullptr);

			//�����첽����˴����߳�
			//@Return : ������ֵ
			int startWorker(void);
			static void workerThreadProc(void* param = nullptr);

			//��Ϣ�ַ�����
			//@src : ����ԴID
			//@dest : Ŀ��ID
			void transferData(void* src = nullptr, void* dest = nullptr);

		protected:
			void* ctx;

		private:
			void* router;
			void* dealer;
			//�߳��˳���ʶ
			bool stopped;
			//������߳���
			//�洢Poller��Worker�߳�ʵ��
			std::vector<void*> threadGroup;
		};//class AsynchronousServer
	}//namespace module
}//namespace mq

#endif//ASYNCHRONOUS_SERVER_H
