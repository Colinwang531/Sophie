//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-08-07
//		Description :					Majordomo协议
//
//		History:						Author									Date										Description
//										王科威									2020-08-07									创建
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

		//Worker在Broker中表示不同的服务模块并且管理与实际服务模块进行通信的相关信息
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
			//一个Worker只提供一个Service
			MDPServicePtr mdpService;
		};//class MajordomoWorkerInfo

		//Service在Broker中表示Broker具备哪些服务内容,这些内容在客户端进行访问时进行消息的转发
		class MDPService
		{
		public:
			MDPService(const std::string name);
			~MDPService(void);

			inline const std::string getName(void) const
			{
				return serviceName;
			}

			//添加Worker实例
			//@worker : worker实例
			void addWorker(MDPWorkerPtr worker);

		private:
			const std::string serviceName;
			//List of client requests
//			std::deque<AbstractMQMessage*> requestGroup;
			//一个Service可以包含多个Worker
			std::list<MDPWorkerPtr> workerGroup;
		};//class MajordomoServiceInfo
	}//namespace module
}//namespace mq

#endif//MQ_MODULE_MAJORDOMO_INFO_H

