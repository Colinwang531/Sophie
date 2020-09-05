//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-08-11
//		Description :					Majordomo�������
//
//		History:						Author									Date										Description
//										������									2020-08-11									����
//

#ifndef MQ_MODULE_MAJORDOMO_WORKER_H
#define MQ_MODULE_MAJORDOMO_WORKER_H

#include "ctx.hpp"
#include "socket_base.hpp"

namespace mq
{
    namespace module
    {
		class MajordomoWorker 
		{
		public:
			MajordomoWorker(
				const std::string name, void* client = nullptr);
			virtual ~MajordomoWorker(void);

		public:
			//����Workerʵ��
			//@address : "tcp://IP:PORT"
			//@Return : ������
			int startWorker(const std::string address);

			//ֹͣWorkerʵ��
			//@Return : ������
			int stopWorker(void);

			//������Ϣ
			//@flagID : Request/Response��ʶ
			//@fromID : ������ID��ʶ
			//@toID : ������ID��ʶ
			//@msg : ��Ϣ����
			//@Return : ������
			int sendData(
				const std::string flagID,
				const std::string fromID,
				const std::string toID,
				const std::string msg);

		private:
			//��Ϣ��ȡ�߳�
			void pollerThreadProc(void);

			//Workerʵ��ע���߳�
			void autoRegisterToMajordomoBrokerThreadProc(void);

		private:
			const std::string workerName;
			void* abstractClient;
			//0MQ context
			zmq::ctx_t ctx;
			//Socket for clients & workers
			zmq::socket_base_t* dealer;
			const std::string workerID;
		};//class MajordomoWorker
    }//namespace module
}//namespace mq

#endif//MQ_MODULE_MAJORDOMO_WORKER_H
