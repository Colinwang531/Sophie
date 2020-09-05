#include "Packet/Message/MessagePacket.h"
using MessagePacket = base::packet::MessagePacket;
#include "Protocol/CommandPhrase.h"
using CommandParser = base::protocol::CommandParser;
#include "Protocol/Device/DevicePhrase.h"
#include "Network/DeviceComponentClient.h"

namespace base
{
	namespace network
	{
		DeviceComponentClient::DeviceComponentClient() : AbstractComponentClient() {}
		DeviceComponentClient::~DeviceComponentClient(){}

		void DeviceComponentClient::afterClientPollMessage(
			const void* data /* = nullptr */, 
			const unsigned int databytes /* = 0 */)
		{
			if (data && 0 < databytes)
			{
				MessagePacket* pkt{
					reinterpret_cast<MessagePacket*>(
						CommandParser().parseFromArray(data, databytes)) };

				if (pkt)
				{
					switch (pkt->getPacketType())
					{
						case base::packet::PacketType::PACKET_TYPE_COMPONENT:
						{
							afterParseDeviceComponentSigninResponse(
								reinterpret_cast<const char*>(pkt->getPayloadData()));
							break;
						}
						case base::packet::PacketType::PACKET_TYPE_DEVICE:
						{
							const base::protocol::DeviceCommand command{ 
								static_cast<base::protocol::DeviceCommand>(pkt->getPayloadCommand()) };
							if (base::protocol::DeviceCommand::DEVICE_COMMAND_NEW_REQ == command)
							{
								afterParseDeviceCommandNewRequest(pkt->getPayloadData());
							}

							break;
						}
						default:
						{
							break;
						}
					}
				}

				boost::checked_delete(pkt);
			}
		}

// 		void DeviceComponentClient::parseComponentMessage(void* pkt /* = nullptr */)
// 		{
			//解析组件注册的应答消息
//			MessagePacket* mp{ reinterpret_cast<MessagePacket*>(pkt) };
// 			ComponentPacket* cp{ reinterpret_cast<ComponentPacket*>(mp->packet) };
// 			ComponentResponse* cr{ reinterpret_cast<ComponentResponse*>(cp->data) };
//			setClientID(cr->cid);
//		}

// 		void DeviceComponentClient::parseDeviceMessage(void* pkt /* = nullptr */)
// 		{
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
//		}
	}//namespace network
}//namespace base
