//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-08-18
//		Description :					Majordomo�ͻ�����
//
//		History:						Author									Date										Description
//										������									2020-08-18									����
//

#ifndef MQ_MODULE_MAJORDOMO_CLIENT_H
#define MQ_MODULE_MAJORDOMO_CLIENT_H

#include "ctx.hpp"
#include "socket_base.hpp"

namespace mq
{
    namespace module
    {
		class MajordomoClient
		{
		public:
			MajordomoClient(void* client = nullptr);
			virtual ~MajordomoClient(void);

		public:
			//����Clientʵ��
			//@address : "tcp://IP:PORT"
			//@Return : ������
			int startClient(const std::string address);

			//ֹͣClientʵ��
			//@Return : ������
			int stopClient(void);

		private:
			//��Ϣ��ȡ�߳�
			void pollerThreadProc(void);

		private:
			void* abstractClient;
			//0MQ context
			zmq::ctx_t ctx;
			//Socket for clients & workers
			zmq::socket_base_t* dealer;
			const std::string clientID;
		};//class MajordomoClient
    }//namespace module
}//namespace mq

#endif//MQ_MODULE_MAJORDOMO_CLIENT_H
