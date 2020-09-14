#include "Packet/DataPacket.h"

namespace base
{
	namespace packet
	{
		DataPacket::DataPacket(
			const DataPacketType type /* = DataPacketType::DATA_PACKET_TYPE_NONE */)
			: packetType{ type }, packetSequence{ -1 }, packetTimestamp{ 0 }
		{}

		DataPacket::~DataPacket() {}

		void DataPacket::setPacketData(void* data /* = nullptr */)
		{
			if (data)
			{
				packetData.push_back(data);
			}
		}

		void* DataPacket::getPacketData(const int idx /* = 0 */)
		{
			void* data{ nullptr };

			if (idx < packetData.size())
			{
				data = packetData[idx];
			}

			return data;
		}
	}//namespace packet
}//namespace base
