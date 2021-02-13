//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-12-18
//		Description :					Router-Dealer抽象类
//										2种模式:
//										1.Router和Dealer都监听,即服务分发;
//										2.Router监听、Dealer连接,即服务代理。
//
//		History:						Author									Date										Description
//										王科威									2020-12-18									创建
//										王科威									2020-12-28									1.规范名字空间
//																															2.重构类设计
//

#ifndef FRAMEWORK_NETWORK_ZMQ_RD_H
#define FRAMEWORK_NETWORK_ZMQ_RD_H

namespace framework
{
	namespace network
	{
		namespace zeromq
		{
			class Msg;

			class RD
			{
			public:
				RD(void);
				virtual ~RD(void);

			public:
				//启动RD
				//@rlp : Router监听端口号
				//@dcip : Dealer连接IPv4地址
				//@dcp : Dealer连接端口号
				//@Return : 错误码
				virtual int startRD(
					const unsigned short rlp = 0,
					const char* dcip = nullptr,
					const unsigned short dcp = 0);

				//启动RD
				//@rlp : Router监听端口号
				//@dlp : Dealer监听端口号
				//@Return : 错误码
				virtual int startRD(
					const unsigned short rlp = 0,
					const unsigned short dlp = 0);

				//停止RD
				//@Return : 错误码
				virtual int stopRD(void);

				//发送消息
				//@idx : Socket索引号
				//@msg : 消息数据
				//@Return : 错误码
				int sendMsg(
					const int idx = -1,
					Msg* msg = nullptr);

			protected:
				//Router端消息接收处理
				//@sender : 发送者ID标识
				//@module : 发送者模型标识
				//@from : 发送者名称
				//@to : 接收者名称
				//@routers : 消息路由表
				//@messages : 消息内容
				virtual void afterParsePolledMessage(
					const std::string sender,
					const std::string module,
					const std::string from,
					const std::string to,
					const std::vector<std::string> routers,
					const std::vector<std::string> messages) = 0;

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
				enum
				{
					SO_NUMBER = 2
				};
				void* sockets[SO_NUMBER];
				bool stopped;
			};//class RD
		}//namespace zeromq
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ZMQ_RD_H
