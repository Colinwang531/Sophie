//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-07
//		Description :					Dealer模型类
//
//		History:						Author									Date										Description
//										王科威									 2021-06-07									 创建
//

#ifndef FRAMEWORK_LIBNETWORK_ZMQ_MODULE_WORKER_H
#define FRAMEWORK_LIBNETWORK_ZMQ_MODULE_WORKER_H

#include <string>

namespace framework
{
	namespace libnetwork
	{
		namespace zmq
		{
			namespace module
			{
				class IWorker;

				class Worker
				{
				public:
					Worker(void);
					virtual ~Worker(void);

				public:
					//启动
					//@interval : 定时器任务执行时间间隔，以秒为单位
					//@Return : 错误码
					virtual int start(const unsigned long long interval = 30);

					//停止
					//@ctx : 上下文实例
					//@Return : 错误码
					virtual int stop(void* ctx = nullptr);

				protected:
					//设置Dealer连接地址和端口
					//@ipv4 : ipv4地址
					//@port : 端口
					//@ctx : 上下文实例
					//@Return : 错误码
					int connect(
						const std::string ipv4,
						const unsigned short port = 0,
						void* ctx = nullptr);

					//Dealer端数据接收处理
					//@data : 消息数据
					virtual void afterDealerPollDataProcess(
						const std::string data) = 0;

					//定时任务处理
					virtual void afterTimerExpiredProcess(void) = 0;

				private:
					IWorker* worker;
				};//class Worker
			}//namespace module
		}//namespace zmq
	}//namespace libnetwork
}//namespace framework

#endif//FRAMEWORK_LIBNETWORK_ZMQ_MODULE_WORKER_H
