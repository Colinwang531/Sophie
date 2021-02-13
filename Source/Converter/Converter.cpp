#include "boost/shared_ptr.hpp"
#include "boost/checked_delete.hpp"
#include "boost/pointer_cast.hpp"
#include "Error.h"
#include "Data/Frame/Frame.h"
using Frame = framework::data::Frame;
using FramePtr = boost::shared_ptr<Frame>;
#include "Converter/FFmpeg/YV12ToYUV420PConverter.h"
#include "Converter/FFmpeg/ImageX2YConverter.h"
#include "Converter/Converter.h"

namespace framework
{
	namespace multimedia
	{
		Converter::Converter() : cvter{ nullptr }
		{}
		Converter::~Converter()
		{
			boost::checked_delete(cvter);
		}

		int Converter::inputData(DataPtr data)
		{
			int e{ data ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				FramePtr fp{
					boost::dynamic_pointer_cast<Frame>(data) };
				const framework::data::FrameType ft{ fp->getFrameType() };

				if (framework::data::FrameType::FRAME_TYPE_YV12 == ft)
				{
					YV12ToYUV420PConverter* cp{ reinterpret_cast<YV12ToYUV420PConverter*>(cvter) };

					if (!cp)
					{
						cp = new(std::nothrow) YV12ToYUV420PConverter;
						cvter = cp;
					}

					e = cp ? cp->inputData(data) : eBadNewObject;
				}
				else if (framework::data::FrameType::FRAME_TYPE_YUV420P == ft)
				{
					ImageX2YConverter* cp{ reinterpret_cast<ImageX2YConverter*>(cvter) };

					if (!cp)
					{
						cp = new(std::nothrow) ImageX2YConverter;
						cvter = cp;
					}

					e = cp ? cp->inputData(data) : eBadNewObject;
				}
				else
				{
					e = eNotSupport;
				}
			}

			return e;
		}
	}//namespace stream
}//namespace base
