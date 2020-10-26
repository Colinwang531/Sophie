//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-15
//		Description :					客户端类
//
//		History:						Author									Date										Description
//										王科威									2020-09-15									创建
//

#ifndef BASE_NETWORK_ABSTRACT_WORKER_H
#define BASE_NETWORK_ABSTRACT_WORKER_H

#include "AbstractClient.h"

namespace base
{
	namespace network
	{
		class AbstractWorker : public AbstractClient
		{
		public:
			AbstractWorker(void);
			virtual ~AbstractWorker(void);

		public:
			//获取UUID标识
			//@Return : UUID标识
			inline const std::string getUUID(void) const
			{
				return uuid;
			}

			//创建UUID标识
			//@name : 服务名称
			//@Comment : 读取Config.xml中的ID标识,如果失败则创建新的UUID标识
			void generateUUIDWithName(const std::string name);

		protected:
			int createNewClient(const std::string) override;
			//发送自动注册/心跳请求消息
			virtual void sendRegisterWorkerServerMessage(void) = 0;
			virtual void sendQuerySystemServiceMessage(void);

		private:
			//事务定时处理线程
			void processTaskTimerThread(void);

 		private:
			//保存UUID免得不断读取文件
			std::string uuid;
		};//class AbstractMediaStreamClient
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_ABSTRACT_WORKER_H
