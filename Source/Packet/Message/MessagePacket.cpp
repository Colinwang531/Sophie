#include "boost/checked_delete.hpp"
#include "Error.h"
#include "Component/AbstractComponent.h"
using AbstractComponent = base::component::AbstractComponent;
#include "Packet/Message/MessagePacket.h"

namespace base
{
	namespace packet
	{
		MessagePacket::MessagePacket(
			const MessagePacketType type /* = MessagePacketType::MESSAGE_PACKET_TYPE_NONE */)
			: DataPacket(DataPacketType::DATA_PACKET_TYPE_MESSAGE), 
			messagePacketCommand{ -1 }, messagePacketType{ type }, messageStatus{ eSuccess }
		{}

		MessagePacket::~MessagePacket()
		{
			cleanupPacketData();
		}

		void MessagePacket::cleanupPacketData()
		{
			if (MessagePacketType::MESSAGE_PACKET_TYPE_ALARM == messagePacketType)
			{
			}
			else if (MessagePacketType::MESSAGE_PACKET_TYPE_ALGORITHM == messagePacketType)
			{
			}
			else if (MessagePacketType::MESSAGE_PACKET_TYPE_COMPONENT == messagePacketType)
			{
				for (int i = 0; i != packetData.size(); ++i)
				{
//					boost::checked_delete<AbstractComponent>(reinterpret_cast<AbstractComponent*>(packetData[i]));
				}
//				packetData.clear();
			}
			else if (MessagePacketType::MESSAGE_PACKET_TYPE_CREW == messagePacketType)
			{
			}
			else if (MessagePacketType::MESSAGE_PACKET_TYPE_DEVICE == messagePacketType)
			{
			}
			else if (MessagePacketType::MESSAGE_PACKET_TYPE_STATUS == messagePacketType)
			{
			}
			else if (MessagePacketType::MESSAGE_PACKET_TYPE_USER == messagePacketType)
			{
			}
			else if (MessagePacketType::MESSAGE_PACKET_TYPE_EVENT == messagePacketType)
			{
			}
		}
	}//namespace packet
}//namespace base
