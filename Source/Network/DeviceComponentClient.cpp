#include "Network/DeviceComponentClient.h"

namespace base
{
	namespace network
	{
		DeviceComponentClient::DeviceComponentClient() : NetworkAsynchronousClient() {}
		DeviceComponentClient::~DeviceComponentClient(){}

		void DeviceComponentClient::parseComponentMessage(void* pkt /* = nullptr */)
		{
			//解析组件注册的应答消息
//			MessagePacket* mp{ reinterpret_cast<MessagePacket*>(pkt) };
// 			ComponentPacket* cp{ reinterpret_cast<ComponentPacket*>(mp->packet) };
// 			ComponentResponse* cr{ reinterpret_cast<ComponentResponse*>(cp->data) };
//			setClientID(cr->cid);
		}

		void DeviceComponentClient::parseDeviceMessage(void* pkt /* = nullptr */)
		{
// 			MessagePacket* mp{ reinterpret_cast<MessagePacket*>(pkt) };
// 			DevicePacket* dp{ reinterpret_cast<DevicePacket*>(mp->packet) };
// 			DeviceRequest* dr{ reinterpret_cast<DeviceRequest*>(dp->data) };
// 
// 			if (DeviceCommand::DEVICE_COMMAND_NEW_REQ == dp->command)
// 			{
// 				afterParseDeviceCommandNewRequest(
// 					dr->did, dr->embedded.ip, dr->embedded.port, dr->embedded.name, dr->embedded.password);
// 			}
// 			else if (DeviceCommand::DEVICE_COMMAND_DELETE_REQ == dp->command)
// 			{
// 				afterParseDeviceCommandDeleteRequest(dr->did);
// 			}
		}
	}//namespace network
}//namespace base
