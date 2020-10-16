#include "boost/make_shared.hpp"
#include "boost/thread.hpp"
#include "Error.h"
#include "Filter/AbstractFilter.h"
#include "Decoder/SDK/HikvisionSDKDecoder.h"

namespace base
{
	namespace stream
	{
		HikvisionSDKDecoder::HikvisionSDKDecoder(AbstractFilter& filter)
			: AbstractDecoder(filter), decoderID{ -1 }, decodeFrameNumber{ 0 }
		{}

		HikvisionSDKDecoder::~HikvisionSDKDecoder(void)
		{
			if (-1 < decoderID)
			{
				PlayM4_Stop(decoderID);
				PlayM4_CloseStream(decoderID);
				PlayM4_FreePort(decoderID);
			}
		}

		int HikvisionSDKDecoder::inputData(StreamPacketPtr pkt)
		{
			int e{ pkt ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				streamPacketGroup.pushBack(pkt);
			}

			return e;
		}

		void HikvisionSDKDecoder::processingDecodeData()
		{
			StreamPacketPtr pkt{ streamPacketGroup.front() };

			if (pkt)
			{
				const base::packet::StreamFrameType type{ pkt->getStreamFrameType() };

				//头信息且未初始化时执行
				if (base::packet::StreamFrameType::STREAM_FRAME_TYPE_HEADER == type && 0 > decoderID)
				{
					if (PlayM4_GetPort(&decoderID))
					{
						PlayM4_SetStreamOpenMode(decoderID, STREAME_REALTIME);
						PlayM4_SetDecCallBackExMend(decoderID, &HikvisionSDKDecoder::afterDecodeOneFrameCallback, NULL, 0, this);
						PlayM4_OpenStream(decoderID, (BYTE*)pkt->getStreamData(), pkt->getStreamDataBytes(), 1048576);
						//HARD_DECODE_ENGINE / SOFT_DECODE_ENGINE
						PlayM4_SetDecodeEngineEx(decoderID, SOFT_DECODE_ENGINE);
						PlayM4_Play(decoderID, NULL);
					}
				}
				else if (base::packet::StreamFrameType::STREAM_FRAME_TYPE_STREAM_DATA == type && -1 < decoderID)
				{
					PlayM4_InputData(decoderID, (BYTE*)pkt->getStreamData(), pkt->getStreamDataBytes());
				}

				streamPacketGroup.popFront();
			}
			else
			{
				//如果没有数据还是休息一下
				boost::this_thread::sleep(boost::posix_time::milliseconds(1));
			}
		}

		void HikvisionSDKDecoder::processInputDataNotification(StreamPacketPtr pkt)
		{
			if (0 == decodeFrameNumber++ % 3)
			{
				abstractFilter.afterProcessInputDataNotification(pkt);
			}
		}

		void HikvisionSDKDecoder::afterDecodeOneFrameCallback(
			long id, char* data, long bytes, FRAME_INFO* frame, void* ctx, void*)
		{
			HikvisionSDKDecoder* decoder{ 
				reinterpret_cast<HikvisionSDKDecoder*>(ctx) };

			if (T_YV12 == frame->nType)
			{
				StreamPacketPtr pkt{ 
					boost::make_shared<StreamPacket>(
						base::packet::StreamPacketType::STREAM_PACKET_TYPE_PRIVATE_HK,
						base::packet::StreamFrameType::STREAM_FRAME_TYPE_YV12) };

				if (pkt)
				{
					pkt->setStreamData((unsigned char*)data, bytes);
					pkt->setImageWithAndHeight(frame->nWidth, frame->nHeight);
					decoder->processInputDataNotification(pkt);
				}
			}
			else if (T_AUDIO16 == frame->nType)
			{
			}
		}
	}//namespace stream
}//namespace base
