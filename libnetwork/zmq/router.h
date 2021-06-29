//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-04
//		Description :					Router类
//
//		History:						Author									Date										Description
//										王科威									2021-06-04									创建
//

#ifndef FRAMEWORK_LIBNETWORK_ZMQ_ROUTER_H
#define FRAMEWORK_LIBNETWORK_ZMQ_ROUTER_H

#include <string>

namespace framework
{
    namespace libnetwork
    {
		namespace zmq
		{
			typedef void (*RouterPollDataHandler)(const std::string, const std::string, void*);
			class IRouter;

			class Router
			{
			public:
				//@c : 上下文实例
				//@handler : 数据读取回调
				//@param : 用户数据
				Router(
					void* c = nullptr,
					RouterPollDataHandler handler = nullptr,
					void* param = nullptr);
				~Router(void);

			public:
				//启动
				//@localIP : 监听IP
				//@localPort : 监听端口
				//@Return : 错误码
				//@Comment : 如果IPv4地址为空则监听所有地址的端口
				int start(
					const std::string localIP,
					const unsigned short localPort = 0);

				//停止
				//@Return : 错误码
				int stop(void);

				//发送数据
				//@targetID : 接收端ID标识
				//@data : 数据内容
				//@Return : 错误码
				int send(
					const std::string targetID,
					const std::string data);

			private:
				IRouter* router;
			};//class Router
		}//namespace zmq
    }//namespace libnetwork
}//namespace framework

#endif//FRAMEWORK_LIBNETWORK_ZMQ_ROUTER_H
