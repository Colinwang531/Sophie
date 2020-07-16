//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-15
//		Description :					异步通信客户端类
//
//		History:						Author									Date										Description
//										王科威									2020-05-15									创建
//										王科威									2020-06-29									添加读/写文件的方法
//																															部分参数需要进行持久化操作
//

#ifndef NETWORK_ASYNCHRONOUS_CLIENT_H
#define NETWORK_ASYNCHRONOUS_CLIENT_H

#include "Mutex/RWLock.h"
#include "MessageQueue/AsynchronousClient.h"
using AsynchronousClient = mq::module::AsynchronousClient;

namespace base
{
	namespace network
	{
		class NetworkAsynchronousClient : public AsynchronousClient
		{
		public:
			NetworkAsynchronousClient(void);
			virtual ~NetworkAsynchronousClient(void);

		protected:
			int startPoller(void) override;

			//数据处理接口
			//@data : 数据内容字符串
			//@databytes : 数据内容字节数
			void afterClientPollMessage(
				const void* data = nullptr, 
				const unsigned int databytes = 0) override;

			//获取客户端ID标识,即组件ID标识
			//@Return : 参数值
			virtual const std::string getClientID(void) const = 0;

			//设置客户端ID标识
			//@value : 参数值
			//@Return : 错误码
			virtual int setClientID(const std::string ID) const = 0;

			//获取客户端名称
			//@Return : 参数值
			virtual const std::string getClientName(void) const = 0;

			//设置客户端名称
			//@value : 参数值
			//@Return : 错误码
			virtual int setClientName(const std::string name) const = 0;

			//获取客户端类型,即组件类型
			//类型由继承者指定,无需提供设置方法
			//@Return : 客户端类型
			//			类型值参见ComponentType定义
			virtual int getClientType(void) const = 0;

			//客户端消息解析处理
			//@pkt : 消息包
			virtual void unpackMessageFailure(void) = 0;
			virtual void parseUnknownMessage(void) = 0;
			virtual void parseAlarmMessage(void* pkt = nullptr) = 0;
			virtual void parseAlgorithmMessage(void* pkt = nullptr) = 0;
			virtual void parseComponentMessage(void* pkt = nullptr) = 0;
			virtual void parseCrewMessage(void* pkt = nullptr) = 0;
			virtual void parseDeviceMessage(void* pkt = nullptr) = 0;
			virtual void parseStatusMessage(void* pkt = nullptr) = 0;
			virtual void parseUserMessage(void* pkt = nullptr) = 0;

		private:
			//创建客户端登录和心跳工作线程
			//@Return : 错误码
			int createNewSigninAndHeartbeatWorkerThread(void);

			//登录和心跳处理
			void signinAndHeartbeatThreadProc(void);

			//获取请求消息序列号
			//@Return : 消息序列号
			const long long getRequestSequenceNumber(void);

			//发送注册或心跳请求消息
			//@Return : 错误码
			int sendSigninOrHeartbeatRequestMessage(void);

		private:
			long long requestSequence;
			SharedMutex mtx;
		};//class NetworkAsynchronousClient
	}//namespace network
}//namespace base

#endif//NETWORK_ASYNCHRONOUS_CLIENT_H
