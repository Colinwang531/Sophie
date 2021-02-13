#include "boost/bind.hpp"
#include "boost/pointer_cast.hpp"
#include "boost/shared_ptr.hpp"
#include "Error.h"
#include "Data/Frame/Frame.h"
using Frame = framework::data::Frame;
using FramePtr = boost::shared_ptr<Frame>;
#include "Encoder/JPEG/JPEGImageEncoder.h"
#include "Encoder/Encoder.h"

namespace framework
{
	namespace multimedia
	{
		Encoder::Encoder() : encoder{ nullptr }
		{}
		Encoder::~Encoder()
		{}

		int Encoder::inputData(DataPtr data)
		{
			int e{ data ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				FramePtr fp{ boost::dynamic_pointer_cast<Frame>(data) };
				const framework::data::FrameType ft{ fp->getFrameType() };

				if (framework::data::FrameType::FRAME_TYPE_BGR == ft)
				{
					JPEGImageEncoder* ep{ reinterpret_cast<JPEGImageEncoder*>(encoder) };

					if (!ep)
					{
						ep = new(std::nothrow) JPEGImageEncoder(
							boost::bind(&Encoder::afterDataEncodeCallback, this, _1));
						encoder = ep;
					}

					e = ep ? ep->inputData(data) : eBadNewObject;
				}
				else
				{
					e = eNotSupport;
				}
			}

			return e;
		}

		void Encoder::afterDataEncodeCallback(DataPtr data)
		{
			if (data)
			{
				afterDataEncodeNotification(data);
			}
		}
	}//namespace multimedia
}//namespace framework