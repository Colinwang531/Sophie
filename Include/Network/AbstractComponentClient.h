//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-15
//		Description :					抽象组件客户端类
//
//		History:						Author									Date										Description
//										王科威									2020-05-15									创建
//										王科威									2020-06-29									添加读/写文件的方法
//																															部分参数需要进行持久化操作
//

#ifndef BASE_NETWORK_NETWORK_ASYNCHRONOUS_CLIENT_H
#define BASE_NETWORK_NETWORK_ASYNCHRONOUS_CLIENT_H

#include "Mutex/RWLock.h"
#include "MessageQueue/AbstractMQClient.h"
using AbstractMQClient = mq::module::AbstractMQClient;

namespace base
{
	namespace network
	{
		class AbstractComponentClient : public AbstractMQClient
		{
		public:
			AbstractComponentClient(void);
			virtual ~AbstractComponentClient(void);

		protected:
			int startPoller(void) override;

			//获取组件客户端ID标识
			//@Return : ID标识
			//@Comment : 继承者对ID标识的持久化处理可以各不相同,由继承者实现
			virtual const std::string getAbstractComponentClientID(void) const = 0;

			//设置组件客户端ID标识
			//@cid : ID标识
			//@Comment : 继承者对ID标识的持久化处理可以各不相同,由继承者实现
			virtual void setAbstractComponentClientID(const std::string cid) = 0;

			//获取组件客户端名称
			//@Return : 名称
			//@Comment : 继承者对名称的持久化处理可以各不相同,由继承者实现
			virtual const std::string getAbstractComponentClientName(void) const = 0;

			//设置组件客户端名称
			//@name : 名称
			//@Comment : 继承者对名称的持久化处理可以各不相同,由继承者实现
			virtual void setAbstractComponentClientName(const std::string name) = 0;

			//获取组件客户端类型
			//@Return : 类型
			//			类型值参见ComponentType定义
			virtual int getAbstractComponentClientType(void) const = 0;

		private:
			//组件客户端自动注册/心跳线程
			void checkAbstractComponentClientOnlineThreadProc(void);

			//获取请求消息序列号
			//@Return : 消息序列号
			const long long getRequestSequenceNumber(void);

			//发送组件客户端注册/心跳请求消息
			//@Return : 错误码
			int sendAbstractComponentClientOnlineRequestMessage(void);

		private:
			long long requestSequence;
			SharedMutex mtx;
		};//class NetworkAsynchronousClient
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_NETWORK_ASYNCHRONOUS_CLIENT_H
