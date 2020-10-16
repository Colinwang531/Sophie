#include <cstring>
#include "boost/checked_delete.hpp"
#include "Error.h"
#include "Packet/Stream/StreamPacket.h"

namespace base
{
	namespace packet
	{
		StreamPacket::StreamPacket(
			const StreamPacketType pktType /* = StreamPacketType::STREAM_PACKET_TYPE_NONE */,
			const StreamFrameType frameType /* = StreamFrameType::STREAM_FRAME_TYPE_NONE */)
			: DataPacket(DataPacketType::DATA_PACKET_TYPE_VIDEO), streamPacketType{ pktType }, streamFrameType{ frameType },
			streamPacketData{ nullptr }, streamPacketDataBytes{ 0 }, imageWidth{ 0 }, imageHeight{ 0 }, alarmX{ 0 }, alarmY{ 0 }, alarmW{ 0 }, alarmH{ 0 }
		{}

		StreamPacket::~StreamPacket()
		{
			boost::checked_array_delete(streamPacketData);
		}

		int StreamPacket::setStreamData(
			const unsigned char* data /* = nullptr */,
			const int bytes /* = 0 */)
		{
			int e{ data && 0 < bytes ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				boost::checked_array_delete(streamPacketData);
				streamPacketData = new(std::nothrow) unsigned char[bytes];

				if (streamPacketData)
				{
#ifdef WINDOWS
					memcpy_s(streamPacketData, bytes, data, bytes);
#else
					memcpy(streamPacketData, data, bytes);
#endif//WINDOWS
					streamPacketDataBytes = bytes;
				}
				else
				{
					e = eBadNewObject;
				}
			}

			return e;
		}

		int StreamPacket::buildStreamData(
			const unsigned char* data /* = nullptr */, 
			const int bytes /* = 0 */)
		{
			int e{ data && 0 < bytes ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				boost::checked_array_delete(streamPacketData);
				streamPacketDataBytes = StreamHeadSize::eFixedHeadSize + bytes;
				streamPacketData = new(std::nothrow) unsigned char[streamPacketDataBytes];

				if (streamPacketData)
				{
					const int headFlag = StreamHeadSize::eFixedHeadFlag;
					const int dataType = static_cast<int>(getPacketType());
					const int streamType = static_cast<int>(getStreamPacketType());
					const int frameType = static_cast<int>(getStreamFrameType());
					const long long sequence{ getPacketSequence() }, timestamp{ getPacketTimestamp() };

#ifdef WINDOWS
					memcpy_s(streamPacketData, 4, &headFlag, 4);
					memcpy_s(streamPacketData + 4, 4, &dataType, 4);
					memcpy_s(streamPacketData + 8, 4, &streamType, 4);
					memcpy_s(streamPacketData + 12, 4, &frameType, 4);
					memcpy_s(streamPacketData + 16, 4, &bytes, 4);
					memcpy_s(streamPacketData + 20, 8, &sequence, 8);
					memcpy_s(streamPacketData + 28, 8, &timestamp, 8);
					memcpy_s(streamPacketData + 36, bytes, data, bytes);
#else
					memcpy(streamPacketData, &headFlag, 4);
					memcpy(streamPacketData + 4, &dataType, 4);
					memcpy(streamPacketData + 8, &streamType, 4);
					memcpy(streamPacketData + 12, &frameType, 4);
					memcpy(streamPacketData + 16, &bytes, 4);
					memcpy(streamPacketData + 20, &sequence, 8);
					memcpy(streamPacketData + 28, &timestamp, 8);
					memcpy(streamPacketData + 36, data, bytes);
#endif//WINDOWS
				} 
				else
				{
					e = eBadNewObject;
				}
			}

			return e;
		}
	}//namespace packet
}//namespace base
