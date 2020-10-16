#include "boost/make_shared.hpp"
extern "C"
{
#include "libavutil/imgutils.h"
}
#include "Error.h"
#include "Filter/AbstractFilter.h"
#include "Converter/ImageX2YConverter.h"

namespace base
{
	namespace stream
	{
		ImageX2YConverter::ImageX2YConverter(AbstractFilter& filter)
			: AbstractImageConverter(filter), 
			srcAVFrame{ nullptr }, tarAVFrame{ nullptr }, 
			srcData{ nullptr }, tarData{ nullptr }, 
			swsContext{ nullptr }
		{}

		ImageX2YConverter::~ImageX2YConverter()
		{
			destroyBuffer();
		}

		int ImageX2YConverter::inputData(StreamPacketPtr pkt)
		{
			int e{ pkt ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				int w{ 0 }, h{ 0 };
				pkt->getImageWidthAndHeight(w, h);
				const base::packet::StreamFrameType srcFrameType{ pkt->getStreamFrameType() };
				base::packet::StreamFrameType tarFrameType{ base::packet::StreamFrameType::STREAM_FRAME_TYPE_NONE };
				AVPixelFormat srcPixelFmt{ AV_PIX_FMT_NONE }, tarPixelFmt{ AV_PIX_FMT_NONE };

				if (base::packet::StreamFrameType::STREAM_FRAME_TYPE_YUV420P == srcFrameType)
				{
					srcPixelFmt = AV_PIX_FMT_YUV420P;
					tarPixelFmt = AV_PIX_FMT_BGR24;
					tarFrameType = base::packet::StreamFrameType::STREAM_FRAME_TYPE_BGR;
				}
				else if (base::packet::StreamFrameType::STREAM_FRAME_TYPE_BGR == srcFrameType)
				{
					srcPixelFmt = AV_PIX_FMT_BGR24;
					tarPixelFmt = AV_PIX_FMT_YUV420P;
					tarFrameType = base::packet::StreamFrameType::STREAM_FRAME_TYPE_YUV420P;
				}
				else
				{
					e = eNotSupport;
				}

				if (eSuccess == e)
				{
					if (!swsContext && !srcAVFrame && !tarAVFrame && !srcData && !tarData && 0 < w && 0 < h)
					{
						e = createNewBuffer(srcPixelFmt, tarPixelFmt, w, h);
					}

					if (eSuccess == e)
					{
						av_image_fill_arrays(
							srcAVFrame->data, srcAVFrame->linesize, (const uint8_t*)pkt->getStreamData(), srcPixelFmt, w, h, 1);
						int height{
							sws_scale(
								swsContext, (uint8_t const* const*)srcAVFrame->data, srcAVFrame->linesize, 0, h, tarAVFrame->data, tarAVFrame->linesize) };

						if (0 < height)
						{
							StreamPacketPtr streampkt{
								boost::make_shared<StreamPacket>(
									pkt->getStreamPacketType(),
									tarFrameType) };

							if (streampkt)
							{
								streampkt->setStreamData(tarData, av_image_get_buffer_size(tarPixelFmt, w, h, 1));
								streampkt->setImageWithAndHeight(w, h);
								if (base::packet::StreamFrameType::STREAM_FRAME_TYPE_BGR == srcFrameType)
								{
									int x{ 0 }, y{ 0 }, w{ 0 }, h{ 0 };
									pkt->getAlarmRange(x, y, w, h);
									streampkt->setAlarmRange(x, y, w, h);
								}
								abstractFilter.afterProcessInputDataNotification(streampkt);
							}
						}
					}
				}
			}

			return e;
		}

		int ImageX2YConverter::createNewBuffer(
			const AVPixelFormat src /* = AV_PIX_FMT_NONE */, 
			const AVPixelFormat tar /* = AV_PIX_FMT_NONE */, 
			const int w /* = 1920 */, 
			const int h /* = 1080 */)
		{
			int e{ 
				AV_PIX_FMT_NONE != src && 
				AV_PIX_FMT_NONE != tar && 
				0 < w && 
				0 < h ? eSuccess : eInvalidParameter};

			if (eSuccess == e)
			{
				srcAVFrame = av_frame_alloc();
				tarAVFrame = av_frame_alloc();
				swsContext = sws_getContext(w, h, src, w, h, tar, SWS_BICUBIC, NULL, NULL, NULL);
				srcData = reinterpret_cast<unsigned char*>(av_malloc(av_image_get_buffer_size(src, w, h, 1)));
				tarData = reinterpret_cast<unsigned char*>(av_malloc(av_image_get_buffer_size(tar, w, h, 1)));

				if (srcAVFrame && tarAVFrame && swsContext && srcData && tarData)
				{
					//只填充输出区域一次
					av_image_fill_arrays(tarAVFrame->data, tarAVFrame->linesize, (const uint8_t*)tarData, tar, w, h, 1);
				}
				else
				{
					destroyBuffer();
					e = eBadNewObject;
				}
			}

			return e;
		}

		int ImageX2YConverter::destroyBuffer()
		{
			sws_freeContext(swsContext);
			av_frame_free(&srcAVFrame);
			av_frame_free(&tarAVFrame);
			av_free(srcData);
			av_free(tarData);
			return eSuccess;
		}
	}//namespace stream
}//namespace base
