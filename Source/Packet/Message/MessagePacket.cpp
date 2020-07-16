#include "boost/checked_delete.hpp"
#include "Packet/Message/MessagePacket.h"

namespace base
{
	namespace packet
	{
		MessagePacket::MessagePacket(
			const PacketType type /* = PacketType::PACKET_TYPE_NONE */,
			const int command /* = 0 */,
			const int result /* = 0 */)
			: AbstractPacket(type), messageCommand{ command }, replyResult{ result }, messageData{ nullptr }
		{}

		MessagePacket::~MessagePacket()
		{}
	}//namespace packet
}//namespace base
