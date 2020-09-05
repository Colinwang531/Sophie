//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-06-28
//		Description :					设备组件客户端类
//
//		History:						Author									Date										Description
//										王科威									2020-06-28									创建
//

#ifndef BASE_NETWORK_DEVICE_COMPONENT_CLIENT_H
#define BASE_NETWORK_DEVICE_COMPONENT_CLIENT_H

#include "Network/AbstractComponentClient.h"
using AbstractComponentClient = base::network::AbstractComponentClient;

namespace base
{
	namespace network
	{
		class DeviceComponentClient : public AbstractComponentClient
		{
		public:
			DeviceComponentClient(void);
			virtual ~DeviceComponentClient(void);

		protected:
			//数据处理接口
			//@data : 数据内容字符串
			//@databytes : 数据内容字节数
			void afterClientPollMessage(
				const void* data = nullptr,
				const unsigned int databytes = 0) override;

			//处理设备组件注册应答消息
			//@cid : 组件ID标识
			//@Return : 错误码
			virtual int afterParseDeviceComponentSigninResponse(const std::string cid) = 0;

			//处理新增设备请求消息
			//@device : 设备信息
			//@Return : 错误码
			virtual int afterParseDeviceCommandNewRequest(void* device = nullptr) = 0;

			//处理删除设备请求消息
			//@did : 设备ID标识
			//@Return : 错误码
//			virtual int afterParseDeviceCommandDeleteRequest(const std::string did) = 0;
		};//class DeviceComponentClient
	}//namespce network
}//namespace base

#endif//BASE_NETWORK_DEVICE_COMPONENT_CLIENT_H
