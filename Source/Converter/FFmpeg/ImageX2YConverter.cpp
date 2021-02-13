extern "C"
{
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
}
#include "boost/pointer_cast.hpp"
#include "boost/shared_ptr.hpp"
#include "Error.h"
#include "Data/Frame/Frame.h"
using Frame = framework::data::Frame;
using FramePtr = boost::shared_ptr<Frame>;
#include "Converter/FFmpeg/ImageX2YConverter.h"

namespace framework
{
	namespace multimedia
	{
		ImageX2YConverter::ImageX2YConverter()
			: iframe{ nullptr }, oframe{ nullptr }, ibuffer{ nullptr }, 
			obuffer{ nullptr }, ibufsize{ 0 }, obufsize{ 0 },ctx{ nullptr }
		{}
		ImageX2YConverter::~ImageX2YConverter()
		{
			SwsContext* c{ reinterpret_cast<SwsContext*>(ctx) };
			AVFrame* iframe_{ reinterpret_cast<AVFrame*>(iframe) };
			AVFrame* oframe_{ reinterpret_cast<AVFrame*>(oframe) };

			sws_freeContext(c);
			av_frame_free(&iframe_);
			av_frame_free(&oframe_);
			av_free(ibuffer);
			av_free(obuffer);
		}

		int ImageX2YConverter::inputData(DataPtr data)
		{
			int e{ data ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				FramePtr fp{
					boost::dynamic_pointer_cast<Frame>(data) };
				int w{ fp->getFrameWidth() }, h{ fp->getFrameHeight() };
				const framework::data::FrameType ft{ fp->getFrameType() };
				AVPixelFormat spf{ AV_PIX_FMT_NONE }, tpf{ AV_PIX_FMT_NONE };

				if (framework::data::FrameType::FRAME_TYPE_YUV420P == ft)
				{
					spf = AV_PIX_FMT_YUV420P;
					tpf = AV_PIX_FMT_BGR24;
				}
				else
				{
					e = eNotSupport;
				}

				if (eSuccess == e)
				{
					if (!ctx && !iframe && !oframe && !ibuffer && !obuffer && 0 < w && 0 < h)
					{
						iframe = av_frame_alloc();
						oframe = av_frame_alloc();
						ctx = sws_getContext(w, h, spf, w, h, tpf, SWS_BICUBIC, NULL, NULL, NULL);
						ibufsize = av_image_get_buffer_size(spf, w, h, 1);
						ibuffer = av_malloc(ibufsize);
						obufsize = av_image_get_buffer_size(tpf, w, h, 1);
						obuffer = av_malloc(obufsize);

						if (ctx && iframe && oframe && ibuffer && obuffer)
						{
							//只填充一次输出区域
							AVFrame* frame{ reinterpret_cast<AVFrame*>(oframe) };
							av_image_fill_arrays(frame->data, frame->linesize, (const uint8_t*)obuffer, tpf, w, h, 1);
						}
					}

					if (ctx && iframe && oframe && ibuffer && obuffer)
					{
						SwsContext* c{ reinterpret_cast<SwsContext*>(ctx) };
						AVFrame* iframe_{ reinterpret_cast<AVFrame*>(iframe) };
						AVFrame* oframe_{ reinterpret_cast<AVFrame*>(oframe) };

						av_image_fill_arrays(iframe_->data, iframe_->linesize, (const uint8_t*)fp->getFrameData(), spf, w, h, 1);
						int height{
							sws_scale(c, iframe_->data, iframe_->linesize, 0, h, oframe_->data, oframe_->linesize) };

						if (0 < height)
						{
							fp->setFrameType(framework::data::FrameType::FRAME_TYPE_BGR);
							e = data->setData(obuffer, obufsize);
						}
					}
				}
			}

			return e;
		}
	}//namespace multimedia
}//namespace framework
