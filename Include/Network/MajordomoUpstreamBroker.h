//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-04-30
//		Description :					Majordomo代理类
//
//		History:						Author									Date										Description
//										王科威									2020-04-30									创建
//										王科威									2020-08-18									1.定义为抽象基类
//										     									          									2.新增服务端类型标识
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

			//服务端数据读取回调
			//@commID : 通信ID标识
			//@roleID : 角色ID标识
			//@flagID : 标志ID标识
			//@fromID : 发送者ID标识
			//@toID : 接收者ID标识
			//@data : 消息数据
			//@Return : 错误码
			virtual void afterPolledDataFromServerCallback(
				const std::string commID,
				const std::string roleID,
				const std::string flagID,
				const std::string fromID,
				const std::string toID,
				const std::string data);

			//WORKER端数据读取回调
			//@roleID : 角色ID标识
			//@flagID : 标志ID标识
			//@fromID : 发送者ID标识
			//@toID : 接收者ID标识
			//@data : 消息数据
			//@Return : 错误码
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
