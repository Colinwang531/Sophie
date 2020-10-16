#include <string>
#include <sstream>
#include "boost/archive/iterators/base64_from_binary.hpp"
#include "boost/archive/iterators/binary_from_base64.hpp"
#include "boost/archive/iterators/transform_width.hpp"
#include "boost/make_shared.hpp"
extern "C"
{
#include "libavutil/imgutils.h"
}
#include "Error.h"
#include "Filter/AbstractFilter.h"
#include "Encoder/JPEG/JPEGImageEncoder.h"

namespace base
{
	namespace stream
	{
		JPEGImageEncoder::JPEGImageEncoder(AbstractFilter& filter)
			: AbstractEncoder(filter), avcodec{ nullptr }, avcodecctx{ nullptr }, srcData{ nullptr }
		{}

		JPEGImageEncoder::~JPEGImageEncoder()
		{}

		int JPEGImageEncoder::inputData(StreamPacketPtr pkt)
		{
			int e{ pkt ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				int w{ 0 }, h{ 0 };
				pkt->getImageWidthAndHeight(w, h);

				if (!avcodec && !avcodecctx && !srcData)
				{
					e = createNewEncoder(w, h);
				}

				if (eSuccess == e)
				{
					av_image_fill_arrays(
						srcData->data, srcData->linesize, (const unsigned char*)pkt->getStreamData(), AV_PIX_FMT_YUV420P, w, h, 1);

					if (!avcodec_send_frame(avcodecctx, srcData))
					{
						AVPacket avpkt;
						av_init_packet(&avpkt);
						if (!avcodec_receive_packet(avcodecctx, &avpkt))
						{
							StreamPacketPtr streampkt{
								boost::make_shared<StreamPacket>(
									pkt->getStreamPacketType(),
									base::packet::StreamFrameType::STREAM_FRAME_TYPE_JPEG) };

							if (streampkt)
							{
								const std::string base64EncodeData{ base64Encode(avpkt.data, avpkt.size) };
								int x{ 0 }, y{ 0 }, w{ 0 }, h{ 0 };
								pkt->getAlarmRange(x, y, w, h);
								streampkt->setAlarmRange(x, y, w, h);
								streampkt->setImageWithAndHeight(w, h);
								streampkt->setStreamData((const unsigned char*)base64EncodeData.c_str(), (int)base64EncodeData.length());
								abstractFilter.afterProcessInputDataNotification(streampkt);
							}
						}
						av_packet_unref(&avpkt);
					}
					else
					{
						e = eBadOperate;
					}
				}
			}

			return e;
		}

		int JPEGImageEncoder::createNewEncoder(const int w /* = 1920 */, const int h /* = 1080 */)
		{
			int e{ 0 < w && 0 < h ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				avcodec = avcodec_find_encoder(AV_CODEC_ID_MJPEG);
				srcData = av_frame_alloc();

				if (avcodec && srcData)
				{
					srcData->format = AV_PIX_FMT_YUV420P;
					srcData->width = w;
					srcData->height = h;

					avcodecctx = avcodec_alloc_context3(avcodec);
					avcodecctx->pix_fmt = AV_PIX_FMT_YUVJ420P;
					avcodecctx->codec_type = AVMEDIA_TYPE_VIDEO;
					avcodecctx->time_base.num = 1;
					avcodecctx->time_base.den = 11;
					avcodecctx->width = w;
					avcodecctx->height = h;

					e = !avcodec_open2(avcodecctx, avcodec, NULL) ? eSuccess : eNotSupport;
				}
				else
				{
					destroyEncoder();
					e = eBadNewObject;
				}
			}

			return e;
		}

		int JPEGImageEncoder::destroyEncoder()
		{
			avcodec_close(avcodecctx);
			av_free(avcodec);
			av_frame_free(&srcData);
			return eSuccess;
		}

		const std::string JPEGImageEncoder::base64Encode(
			const unsigned char* data /* = nullptr */, 
			const int bytes /* = 0 */)
		{
			typedef boost::archive::iterators::base64_from_binary<
				boost::archive::iterators::transform_width<std::string::const_iterator, 6, 8>> Base64EncodeIterator;
			std::string inputData((const char*)data, bytes);
			std::stringstream result;
			copy(Base64EncodeIterator(inputData.begin()), Base64EncodeIterator(inputData.end()), std::ostream_iterator<char>(result));
			size_t equal_count = (3 - inputData.length() % 3) % 3;
			for (size_t i = 0; i < equal_count; i++)
			{
				result.put('=');
			}
			//去掉结尾数据乱码
			result.put('\0');

			return result.str();
		}
	}//namespace stream
}//namespace base
