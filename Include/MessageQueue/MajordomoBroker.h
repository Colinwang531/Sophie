//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-04-26
//		Description :					Majordomo�������
//
//		History:						Author									Date										Description
//										������									2020-04-26									����
//

#ifndef MQ_MODULE_MAJORDOMO_BROKER_H
#define MQ_MODULE_MAJORDOMO_BROKER_H

// #include "DataStruct/UnorderedMap.h"
// #include "MajordomoInfo.h"
#include "MessageData.h"
using MessageData = mq::message::MessageData;

namespace mq
{
    namespace module
    {
		class MajordomoBroker
		{
		public:
			MajordomoBroker(void* server = nullptr);
			virtual ~MajordomoBroker(void);

		public:
			//�����������
			//@address : "tcp://IP:PORT"
			//@Return : ������
			//@Comment :  Bind broker to endpoint, can call this multiple times
			//			  We use a single socket for both clients and workers.
			int startBroker(const std::string address);

			//ֹͣ�������
			//@Return : ������
			int stopBroker(void);

			//��������
			//@commID : ͨ��ID��ʶ
			//@flagID : Request/Response��ʶ
			//@fromID : ������ID��ʶ
			//@toID : ������ID��ʶ
			//@msg : ��Ϣ����
			//@Return : ������
			int sendData(
				const std::string commID, 
				const std::string flagID,
				const std::string fromID,
				const std::string toID,
				const std::string msg);

		private:
			//��Ϣ��ȡ�߳�
			void pollerThreadProc(void);

			//Workerʵ����ʱ����߳�
			void autoCheckWorkerTimeoutThreadProc(void);

		private:
			void* abstractServer;
			//0MQ context
			zmq::ctx_t ctx;
			//Socket for clients & workers
			zmq::socket_base_t* router;
		};//class MajordomoBroker
    }//namespace module
}//namespace mq

#endif//MQ_MODULE_MAJORDOMO_BROKER_H
