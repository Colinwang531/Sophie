//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-04-30
//		Description :					Majordomo������
//
//		History:						Author									Date										Description
//										������									2020-04-30									����
//										������									2020-08-18									1.����Ϊ�������
//										     									          									2.������������ͱ�ʶ
//

#ifndef BASE_NETWORK_MAJORDOMO_UPSTREAM_BROKER_H
#define BASE_NETWORK_MAJORDOMO_UPSTREAM_BROKER_H

#include "boost/shared_ptr.hpp"
#include "MessageQueue/MajordomoBroker.h"
using MajordomoBroker = mq::module::MajordomoBroker;
using MajordomoBrokerPtr = boost::shared_ptr<MajordomoBroker>;
#include "MessageQueue/MajordomoWorker.h"
using MajordomoWorker = mq::module::MajordomoWorker;
using MajordomoWorkerPtr = boost::shared_ptr<MajordomoWorker>;
#include "AbstractServer.h"
#include "AbstractWorker.h"

namespace base
{
	namespace network
	{
		class MajordomoUpstreamBroker 
			: public AbstractServer, public AbstractWorker
		{
		public:
			MajordomoUpstreamBroker(void);
			virtual ~MajordomoUpstreamBroker(void);

		protected:
			int createNewServer(const std::string address) override;
			int destroyServer(void) override;
			int createNewClient( std::string address) override;
			int destroyClient(void) override;
			int sendData(
				const std::string roleID,
				const std::string flagID,
				const std::string fromID,
				const std::string toID,
				const std::string data) override;
			int sendData(
				const std::string commID,
				const std::string roleID,
				const std::string flagID,
				const std::string fromID,
				const std::string toID,
				const std::string data) override;

			//��������ݶ�ȡ�ص�
			//@commID : ͨ��ID��ʶ
			//@roleID : ��ɫID��ʶ
			//@flagID : ��־ID��ʶ
			//@fromID : ������ID��ʶ
			//@toID : ������ID��ʶ
			//@data : ��Ϣ����
			//@Return : ������
			virtual void afterPolledDataFromServerCallback(
				const std::string commID,
				const std::string roleID,
				const std::string flagID,
				const std::string fromID,
				const std::string toID,
				const std::string data);

			//WORKER�����ݶ�ȡ�ص�
			//@roleID : ��ɫID��ʶ
			//@flagID : ��־ID��ʶ
			//@fromID : ������ID��ʶ
			//@toID : ������ID��ʶ
			//@data : ��Ϣ����
			//@Return : ������
			virtual void afterPolledDataFromWorkerCallback(
				const std::string roleID,
				const std::string flagID,
				const std::string fromID,
				const std::string toID,
				const std::string data);

		private:
			MajordomoBrokerPtr broker;
			MajordomoWorkerPtr worker;
		};//class MajordomoUpstreamBroker
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_MAJORDOMO_UPSTREAM_BROKER_H
