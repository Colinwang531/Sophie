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
					Publisher(void);
					virtual ~Publisher(void);

				public:
					//绑定Pub监听地址和端口
					//@ipv4 : ipv4监听地址
					//@port : 监听端口
					//@ctx : 上下文实例
					//@Return : 错误码
					int bind(
						const std::string ipv4,
						const unsigned short port = 0,
						void* ctx = nullptr);

					//发送数据
					//@msg : 消息数据
					//@Return : 错误码
					int send(void* msg = nullptr);

				private:
					IPublisher* publisher;
				};//class Publisher
			}//namespace module
		}//namespace zmq
	}//namespace libnetwork
}//namespace framework

#endif//FRAMEWORK_LIBNETWORK_ZMQ_MODULE_PUBLISHER_H
