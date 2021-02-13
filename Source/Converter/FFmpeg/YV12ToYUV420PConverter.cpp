#include <string.h>
#include "boost/shared_ptr.hpp"
#include "boost/pointer_cast.hpp"
#include "Error.h"
#include "Data/Frame/Frame.h"
using Frame = framework::data::Frame;
using FramePtr = boost::shared_ptr<Frame>;
#include "Converter/FFmpeg/YV12ToYUV420PConverter.h"

namespace framework
{
	namespace multimedia
	{
		YV12ToYUV420PConverter::YV12ToYUV420PConverter()
		{}
		YV12ToYUV420PConverter::~YV12ToYUV420PConverter()
		{}

		int YV12ToYUV420PConverter::inputData(DataPtr data)
		{
			int e{ data ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				FramePtr fp{ 
					boost::dynamic_pointer_cast<Frame>(data) };

				if (framework::data::FrameType::FRAME_TYPE_YV12 == fp->getFrameType())
				{
					int w{ fp->getFrameWidth() }, h{ fp->getFrameHeight() };
					int yStrideBytes = w * h;
					int uvStrideBytes = yStrideBytes / 4;
					int srcUOffset = yStrideBytes;
					int srcVOffset = srcUOffset + uvStrideBytes;
					int dstVOffset = yStrideBytes;
					int dstUOffset = dstVOffset + uvStrideBytes;
					void* buffer{ new(std::nothrow) unsigned char[uvStrideBytes] };
					void* raw{ const_cast<void*>(fp->getFrameData()) };

					if (buffer)
					{
#ifdef WINDOWS
						memcpy_s(buffer, uvStrideBytes, raw + srcVOffset, uvStrideBytes);
						memcpy_s(raw + dstUOffset, uvStrideBytes, raw + srcUOffset, uvStrideBytes);
						memcpy_s(raw + dstVOffset, uvStrideBytes, buffer, uvStrideBytes);
#else
						memcpy(buffer, raw + srcVOffset, uvStrideBytes);
						memcpy(raw + dstUOffset, raw + srcUOffset, uvStrideBytes);
						memcpy(raw + dstVOffset, buffer, uvStrideBytes);
#endif//WINDOWS
						boost::checked_array_delete(buffer);
						fp->setFrameType(framework::data::FrameType::FRAME_TYPE_YUV420P);
					}
					else
					{
						e = eBadNewMemory;
					}
				}
			}

			return e;
		}
	}//namespace multimedia
}//namespace framework
