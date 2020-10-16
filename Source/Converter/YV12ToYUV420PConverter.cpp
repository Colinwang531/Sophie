#include "boost/checked_delete.hpp"
#include "boost/make_shared.hpp"
extern "C"
{
#include "libavutil/imgutils.h"
}
#include "Error.h"
#include "Filter/AbstractFilter.h"
#include "Converter/YV12ToYUV420PConverter.h"

namespace base
{
	namespace stream
	{
		YV12ToYUV420PConverter::YV12ToYUV420PConverter(AbstractFilter& filter)
			: AbstractImageConverter(filter), yuv420pData{ nullptr }, yuv420pDataBytes{ 0 }
		{}

		YV12ToYUV420PConverter::~YV12ToYUV420PConverter()
		{
			destroyBuffer();
		}

		int YV12ToYUV420PConverter::inputData(StreamPacketPtr pkt)
		{
			int e{ pkt ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				int w{ 0 }, h{ 0 };
				pkt->getImageWidthAndHeight(w, h);

				if (!yuv420pData && 0 < w && 0 < h)
				{
					e = createNewBuffer(w, h);
				}

				if (eSuccess == e)
				{
					const unsigned char* imageData{ pkt->getStreamData() };
					int yStrideBytes = w * h;
					int uvStrideBytes = yStrideBytes / 4;
					int srcUOffset = yStrideBytes;
					int srcVOffset = srcUOffset + uvStrideBytes;
					int dstVOffset = yStrideBytes;
					int dstUOffset = dstVOffset + uvStrideBytes;

#ifdef WINDOWS
					memcpy_s(yuv420pData, yStrideBytes, imageData, yStrideBytes);
					memcpy_s(yuv420pData + dstVOffset, uvStrideBytes, imageData + srcVOffset, uvStrideBytes);
					memcpy_s(yuv420pData + dstUOffset, uvStrideBytes, imageData + srcUOffset, uvStrideBytes);
#else
					memcpy(yuv420pData, imageData, yStrideBytes);
					memcpy(yuv420pData + dstVOffset, imageData + srcVOffset, uvStrideBytes);
					memcpy(yuv420pData + dstUOffset, imageData + srcUOffset, uvStrideBytes);
#endif//WINDOWS

					StreamPacketPtr yuv420ppkt{
						boost::make_shared<StreamPacket>(
							pkt->getStreamPacketType(),
							base::packet::StreamFrameType::STREAM_FRAME_TYPE_YUV420P) };

					if (yuv420ppkt)
					{
						yuv420ppkt->setStreamData(yuv420pData, yuv420pDataBytes);
						yuv420ppkt->setImageWithAndHeight(w, h);
						abstractFilter.afterProcessInputDataNotification(yuv420ppkt);
					}
				}
			}

			return e;
		}

		int YV12ToYUV420PConverter::createNewBuffer(const int w /* = 1920 */, const int h /* = 1080 */)
		{
			int e{ 0 < w && 0 < h ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				yuv420pDataBytes = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, w, h, 1);
				yuv420pData = new(std::nothrow) unsigned char[yuv420pDataBytes];
			}

			return yuv420pData ? eSuccess : eBadNewObject;
		}

		int YV12ToYUV420PConverter::destroyBuffer()
		{
			boost::checked_array_delete(yuv420pData);
			yuv420pData = nullptr;
			return eSuccess;
		}
	}//namespace stream
}//namespace base
