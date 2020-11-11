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
			//不能判断data是否为空
			//有的地方需要将数据设置为空
			packetData.push_back(data);
		}

		void DataPacket::setPacketData(const int idx /* = 0 */, void* data /* = nullptr */)
		{
			if (idx < packetData.size() && data)
			{
				packetData[idx] = data;
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
