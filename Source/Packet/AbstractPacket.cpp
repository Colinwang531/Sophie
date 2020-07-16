#include "Time/XTime.h"
using base::time::Time;
#include "Packet/AbstractPacket.h"

namespace base
{
	namespace packet
	{
		AbstractPacket::AbstractPacket(
			const PacketType type /* = PacketType::PACKET_TYPE_NONE */)
			: packetType{ type }, packetSequence{ 0 }, packetTimestamp{ Time().tickcount() }
		{}
		AbstractPacket::~AbstractPacket(){}
	}//namespace packet
}//namespace base
