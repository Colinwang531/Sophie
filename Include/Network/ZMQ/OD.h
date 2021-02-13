//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-12-18
//		Description :					Dealer抽象类
//										单一模式,即Dealer连接
//
//		History:						Author									Date										Description
//										王科威									2020-12-18									创建
//

#ifndef FRAMEWORK_NETWORK_ZMQ_OD_H
#define FRAMEWORK_NETWORK_ZMQ_OD_H

namespace framework
{
	namespace network
	{
		namespace zeromq
		{
			class Msg;

			class OD
			{
			public:
				OD(void);
				virtual ~OD(void);

			public:
				//启动OD
				//@address : IPv4地址
				//@listenPort : 监听端口号
				//@Return : 错误码
				virtual int startOD(
					const char* ipv4 = nullptr,
					const unsigned short port = 0);

				//停止OD
				//@Return : 错误码
				virtual int stopOD(void);

				//发送消息
				//@msg : 消息数据
				//@Return : 错误码
				int sendMsg(Msg* msg = nullptr);

				//获取OD运行状态
				//Return : OD运行状态
				inline const bool isStopped(void) const
				{
					return stopped;
				}

			protected:
				//Dealer端消息接收处理
				//@module : 发送者模型标识
				//@from : 发送者名称
				//@to : 接收者名称
				//@routers : 消息路由表
				//@messages : 消息内容
				virtual void afterParsePolledMessage(
					const std::string module,
					const std::string from,
					const std::string to,
					const std::vector<std::string> routers,
					const std::vector<std::string> messages) = 0;

			private:
				//数据读取线程
				void pollMessageWorkerThread(void);

				//消息接收处理
				//Comment : 按照消息格式将消息解析出来再交给上层处理
				void parsePolledMessage(Msg& msg);

			private:
				void* so;
				bool stopped;
			};//class OD
		}//namespace zeromq
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ZMQ_OD_H
