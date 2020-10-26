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

#include "boost/function.hpp"
#include "ctx.hpp"

namespace mq
{
    namespace module
    {
		typedef boost::function<void(const std::string, const std::string, const std::string, const std::string, const std::string, const std::string)> AfterServerRecievedDataCallback;

		class MajordomoBroker
		{
		public:
			MajordomoBroker(
				AfterServerRecievedDataCallback callback = nullptr);
			virtual ~MajordomoBroker(void);

		public:
			//�����������
			//@address : ����˼�����ַ
			//@Return : ������
			//@Comment :  Bind broker to endpoint, can call this multiple times
			//			  We use a single socket for both clients and workers.
			int startBroker(const std::string address);

			//ֹͣ�������
			//@Return : ������
			int stopBroker(void);

			//��������
			//@commID : ͨ��ID��ʶ
			//@roleID : ��ɫID��ʶ
			//@flagID : ��־ID��ʶ
			//@fromID : ������ID��ʶ
			//@toID : ������ID��ʶ
			//@data : ��Ϣ����
			//@Return : ������
			int sendData(
				const std::string commID,
				const std::string roleID,
				const std::string flagID,
				const std::string fromID,
				const std::string toID,
				const std::string data);

		private:
			//��Ϣ��ȡ�߳�
			void pollerThreadProc(void);

		private:
			AfterServerRecievedDataCallback afterServerRecievedDataCallback;
			bool stopped;
			zmq::ctx_t ctx;
			zmq::socket_base_t* router;
		};//class MajordomoBroker
    }//namespace module
}//namespace mq

#endif//MQ_MODULE_MAJORDOMO_BROKER_H
