//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-05
//		Description :					Router-Dealer模型类
//										该模型支持2种工作模式:
//										1.仅Router监听;
//										2.Router监听且Dealer连接。
//
//		History:						Author									Date										Description
//										王科威									 2021-06-05									 创建
//

#ifndef FRAMEWORK_LIBNETWORK_ZMQ_MODULE_DISPATCHER_H
#define FRAMEWORK_LIBNETWORK_ZMQ_MODULE_DISPATCHER_H

#include <string>

namespace framework
{
	namespace libnetwork
	{
		namespace zmq
		{
			namespace module
			{
				class IDispatcher;

				class Dispatcher
				{
				public:
					Dispatcher(void);
					virtual ~Dispatcher(void);

				public:
					//启动
					//@Return : 错误码
					virtual int start(void);

					//停止
					//@ctx : 上下文实例
					//@Return : 错误码
					virtual int stop(void* ctx = nullptr);

				protected:
					//绑定Router监听地址和端口
					//@ipv4 : ipv4监听地址
					//@port : 监听端口
					//@ctx : 上下文实例
					//@Return : 错误码
					int bind(
						const std::string ipv4,
						const unsigned short port = 0,
						void* ctx = nullptr);

					//设置Dealer连接地址和端口
					//@ipv4 : ipv4地址
					//@port : 端口
					//@ctx : 上下文实例
					//@Return : 错误码
					int connect(
						const std::string ipv4,
						const unsigned short port = 0,
						void* ctx = nullptr);

					//Router端数据接收处理
					//@sender : 发送端ID标识
					//@data : 消息数据
					virtual void afterRouterPollDataProcess(
						const std::string sender,
						const std::string data) = 0;

					//Dealer端数据接收处理
					//@data : 消息数据
					virtual void afterDealerPollDataProcess(
						const std::string data) = 0;

				private:
					IDispatcher* dispatcher;
				};//class Dispatcher
			}//namespace module
		}//namespace zmq
	}//namespace libnetwork
}//namespace framework

#endif//FRAMEWORK_LIBNETWORK_ZMQ_MODULE_DISPATCHER_H
