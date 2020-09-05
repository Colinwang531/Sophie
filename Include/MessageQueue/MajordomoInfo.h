//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-08-07
//		Description :					MajordomoЭ��
//
//		History:						Author									Date										Description
//										������									2020-08-07									����
//

#ifndef MQ_MODULE_MAJORDOMO_INFO_H
#define MQ_MODULE_MAJORDOMO_INFO_H

#include <set>
#include <deque>
#include <list>
// #include "MessageQueue/AbstractMQMessage.h"
// using AbstractMQMessage = mq::message::AbstractMQMessage;
#include "boost/shared_ptr.hpp"

namespace mq
{
	namespace module
	{
		class MDPService;
		class MDPWorker;
		using MDPWorkerPtr = boost::shared_ptr<MDPWorker>;
		using MDPServicePtr = boost::shared_ptr<MDPService>;

		//Worker��Broker�б�ʾ��ͬ�ķ���ģ�鲢�ҹ�����ʵ�ʷ���ģ�����ͨ�ŵ������Ϣ
		class MDPWorker
		{
		public:
			MDPWorker(
				const std::string id, 
				MDPServicePtr service);
			~MDPWorker(void);

			inline void setExpiry(const long long tm = 0)
			{
				expiryTime = tm;
			}
			inline const long long getExpiry(void) const
			{
				return expiryTime;
			}
			inline const MDPServicePtr getService(void) const
			{
				return mdpService;
			}
			inline const std::string getWorkerID(void) const
			{
				return workerID;
			}

		private:
			long long expiryTime;
			const std::string workerID;
			//һ��Workerֻ�ṩһ��Service
			MDPServicePtr mdpService;
		};//class MajordomoWorkerInfo

		//Service��Broker�б�ʾBroker�߱���Щ��������,��Щ�����ڿͻ��˽��з���ʱ������Ϣ��ת��
		class MDPService
		{
		public:
			MDPService(const std::string name);
			~MDPService(void);

			inline const std::string getName(void) const
			{
				return serviceName;
			}

			//���Workerʵ��
			//@worker : workerʵ��
			void addWorker(MDPWorkerPtr worker);

		private:
			const std::string serviceName;
			//List of client requests
//			std::deque<AbstractMQMessage*> requestGroup;
			//һ��Service���԰������Worker
			std::list<MDPWorkerPtr> workerGroup;
		};//class MajordomoServiceInfo
	}//namespace module
}//namespace mq

#endif//MQ_MODULE_MAJORDOMO_INFO_H

