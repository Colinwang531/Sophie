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
				const char* data = nullptr, 
				const unsigned int databytes = 0) override;

			//获取客户端类型
			//该类型由继承者定义
			//@Return : 客户端类型
			//			类型值参见ComponentType定义
			virtual int getClientType(void) const = 0;

			//获取客户端ID标识
			//该类型由继承者定义
			//@Return : 客户端ID标识
			virtual const char* getClientID(void) const = 0;

			//获取客户端名称
			//该类型由继承者定义
			//@Return : 客户端名称
			virtual const char* getClientName(void) const = 0;

			//以下接口由实现者按照应用实例的不同类型各自完成具体处理的过程
			//所有方法在该类中都没有任何实现
			//未知消息处理接口
			virtual void unpackMessageFailure(void);
			//未知消息处理接口
			virtual void parseUnknownMessage(void);
			//报警消息处理接口
			//@pkt : 报警消息
			virtual void parseAlarmMessage(void* pkt = nullptr);
			//算法消息处理接口
			//@pkt : 算法消息
			virtual void parseAlgorithmMessage(void* pkt = nullptr);
			//组件消息处理接口
			//@pkt : 组件消息
			virtual void parseComponentMessage(void* pkt = nullptr);
			//成员消息处理接口
			//@pkt : 成员消息
			virtual void parseCrewMessage(void* pkt = nullptr);
			//设备消息处理接口
			//@pkt : 设备消息
			virtual void parseDeviceMessage(void* pkt = nullptr);
			//状态消息处理接口
			//@pkt : 状态消息
			virtual void parseStatusMessage(void* pkt = nullptr);
			//用户消息处理接口
			//@pkt : 状态消息
			virtual void parseUserMessage(void* pkt = nullptr);

		private:
			//创建客户端登录和心跳工作线程
			//@Return : 错误码
			int createNewSigninAndHeartbeatWorkerThread(void);

			//登录和心跳处理
			void signinAndHeartbeatThreadProc(void);

			//获取请求消息序列号
			//@Return : 消息序列号
			const long long getRequestSequenceNumber(void);

			//发送登录或心跳请求消息
			//@clientID : 客户端ID标识,即组件ID标识
			//			  空表示注册,非空表示心跳
			//@Return : 错误码
			int sendSigninOrHeartbeatRequestMessage(const char* clientID = nullptr);

		private:
			bool isSigned;
			long long requestSequence;
			SharedMutex mtx;
		};//class NetworkAsynchronousClient
	}//namespace network
}//namespace base

#endif//NETWORK_ASYNCHRONOUS_CLIENT_H
