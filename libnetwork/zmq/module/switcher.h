//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-05
//		Description :					交换机模型类
//
//		History:						Author									Date										Description
//										王科威									 2021-06-05									 创建
//

#ifndef FRAMEWORK_LIBNETWORK_ZMQ_MODULE_SWITCHER_H
#define FRAMEWORK_LIBNETWORK_ZMQ_MODULE_SWITCHER_H

#include <string>

namespace framework
{
	namespace libnetwork
	{
		namespace zmq
		{
			namespace module
			{
				class ISwitcher;

				class Switcher
				{
				public:
					//@c : 上下文实例
					Switcher(void* c = nullptr);
					virtual ~Switcher(void);

				public:
					//监听
					//@localIP : 监听IP
					//@localPort : 监听端口
					//@Return : 错误码
					int bind(
						const std::string localIP,
						const unsigned short localPort = 0);

					//发送数据
					//@targetID : 目标ID标识
					//@data : 数据内容
					//@Return : 错误码
					int send(
						const std::string targetID, 
						const std::string data);

				protected:
					//数据接收处理
					//@sourceID : 源ID标识
					//@data : 消息数据
					virtual void afterSwitcherPollDataProcess(
						const std::string sourceID,
						const std::string data) = 0;

				private:
					ISwitcher* switcher;
				};//class Switcher
			}//namespace module
		}//namespace zmq
	}//namespace libnetwork
}//namespace framework

#endif//FRAMEWORK_LIBNETWORK_ZMQ_MODULE_SWITCHER_H
