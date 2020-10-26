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

#include "boost/function.hpp"
#include "ctx.hpp"

namespace mq
{
    namespace module
    {
		typedef boost::function<void(const std::string, const std::string, const std::string, const std::string, const std::string)> AfterClientRecievedDataCallback;

		class MajordomoWorker 
		{
		public:
			MajordomoWorker(
				AfterClientRecievedDataCallback callback = nullptr);
			virtual ~MajordomoWorker(void);

		public:
			//����Workerʵ��
			//@address : ����˼�����ַ
			//@Return : ������
			int startWorker(const std::string address);

			//ֹͣWorkerʵ��
			//@Return : ������
			int stopWorker(void);

			//������Ϣ
			//@roleID : ��ɫID��ʶ
			//@flagID : ��־ID��ʶ
			//@fromID : ������ID��ʶ
			//@toID : ������ID��ʶ
			//@data : ��Ϣ����
			//@Return : ������
			int sendData(
				const std::string roleID,
				const std::string flagID,
				const std::string fromID,
				const std::string toID,
				const std::string data);

		private:
			//��Ϣ��ȡ�߳�
			void pollerThreadProc(void);

		private:
			AfterClientRecievedDataCallback afterClientRecievedDataCallback;
			bool stopped;
			zmq::ctx_t ctx;
			zmq::socket_base_t* dealer;
		};//class MajordomoWorker
    }//namespace module
}//namespace mq

#endif//MQ_MODULE_MAJORDOMO_WORKER_H
