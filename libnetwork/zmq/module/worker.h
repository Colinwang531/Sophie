//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-07
//		Description :					工作者模型类
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
					Worker(void* c = nullptr);
					virtual ~Worker(void);

				public:
					//连接
					//@sourceID : 源ID标识
					//@remoteIP : 远程服务IP
					//@remotePort : 远程服务端口
					//@Return : 错误码
					int connect(
						const std::string sourceID,
						const std::string remoteIP, 
						const unsigned short remotePort = 0);

					//发送数据
					//@data : 数据内容
					//@Return : 错误码
					int send(const std::string data);

				protected:
					//数据接收处理
					//@data : 消息数据
					virtual void afterWorkerPollDataProcess(
						const std::string data) = 0;

				private:
					IWorker* worker;
				};//class Worker
			}//namespace module
		}//namespace zmq
	}//namespace libnetwork
}//namespace framework

#endif//FRAMEWORK_LIBNETWORK_ZMQ_MODULE_WORKER_H
