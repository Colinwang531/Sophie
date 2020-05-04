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

#include <vector>

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
			//@address : ����˵�ַ
			//@port : �˿ں�
			//@Return : ������ֵ
			int startClient(const char* address = nullptr, const unsigned short port = 61001);

			//ֹͣ�ͻ���
			//@Return : ������ֵ
			int stopClient(void);

//			int send(const char* data, const int dataBytes, std::string& resp);

		protected:
			//�����ͻ���ģ��
			//�̳����ڸ÷�����ʵ���Զ���ģ�͵Ĵ���
			//@address : ����˵�ַ
			//@Return : ������ֵ
			virtual int createNewModule(const char* address = nullptr);

			//���ٿͻ���ģ��
			//@Return : ������ֵ
			virtual int destroyModule(void);

			//������ݶ�ȡʵ����
			//@items : ���ݶ�ȡʵ������
			virtual void addPollItem(std::vector<void*>& items);

			//���ݶ�ȡ����
			//@s : ���ݶ�ȡ��socketʵ��
			virtual void afterPollItemMessage(void* s = nullptr);

		private:
			//�����첽�ͻ������ݶ�ȡ
			//@Return : ������ֵ
			int startPoller(void);
			static void pollerThreadProc(void* param = nullptr);
			
		protected:
			void* ctx;

		private:
			void* dealer;
			bool stopped;
			void* workerThread;
		};//class AsynchronousClient
	}//namespace module
}//namespace mq

#endif//ASYNCHRONOUS_CLIENT_H
