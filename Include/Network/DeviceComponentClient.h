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

#include "Network/NetworkAsynchronousClient.h"
using NetworkAsynchronousClient = base::network::NetworkAsynchronousClient;

namespace base
{
	namespace network
	{
		class DeviceComponentClient : public NetworkAsynchronousClient
		{
		public:
			DeviceComponentClient(void);
			virtual ~DeviceComponentClient(void);

		protected:
			void parseAlarmMessage(void* pkt = nullptr) override {};
			void parseAlgorithmMessage(void* pkt = nullptr) override {};
			void parseComponentMessage(void* pkt = nullptr) override;
			void parseCrewMessage(void* pkt = nullptr) override {};
			void parseDeviceMessage(void* pkt = nullptr) override;
			void parseStatusMessage(void* pkt = nullptr) override {};
			void parseUserMessage(void* pkt = nullptr) override {};

			//处理新增设备请求消息
			//@did : 设备ID标识
			//@address : 设备IP地址
			//@port : 设备端口号
			//@name : 设备登录名称
			//@password : 设备登录密码
			//@Return : 错误码
			virtual int afterParseDeviceCommandNewRequest(
				const std::string did, 
				const std::string address,
				const unsigned short port,
				const std::string name,
				const std::string password) = 0;

			//处理删除设备请求消息
			//@did : 设备ID标识
			//@Return : 错误码
			virtual int afterParseDeviceCommandDeleteRequest(const std::string did) = 0;
		};//class DeviceComponentClient
	}//namespce network
}//namespace base

