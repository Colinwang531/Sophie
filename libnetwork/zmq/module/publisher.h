//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-07
//		Description :					PUB模型类
//
//		History:						Author									Date										Description
//										王科威									 2021-06-07									 创建
//

#ifndef FRAMEWORK_LIBNETWORK_ZMQ_MODULE_PUBLISHER_H
#define FRAMEWORK_LIBNETWORK_ZMQ_MODULE_PUBLISHER_H

#include <string>

namespace framework
{
	namespace libnetwork
	{
		namespace zmq
		{
			namespace module
			{
				class IPublisher;

				class Publisher
				{
				public:
					//@c : 上下文实例
					Publisher(void* c = nullptr);
					virtual ~Publisher(void);

				public:
					//监听
					//@localIP : 监听IP
					//@localPort : 监听端口
					//@Return : 错误码
					int bind(
						const std::string localIP,
						const unsigned short localPort = 0);

					//发送数据
					//@data : 数据内容
					//@Return : 错误码
					int send(const std::string data);

				private:
					IPublisher* publisher;
				};//class Publisher
			}//namespace module
		}//namespace zmq
	}//namespace libnetwork
}//namespace framework

#endif//FRAMEWORK_LIBNETWORK_ZMQ_MODULE_PUBLISHER_H
