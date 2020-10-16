#include "boost/make_shared.hpp"
#include "Error.h"
#include "Decoder/SDK/HikvisionSDKDecoder.h"
using HikvisionSDKDecoderPtr = boost::shared_ptr<base::stream::HikvisionSDKDecoder>;
#include "Pin/AbstractPin.h"
#include "Filter/Decoder/StreamDecoderFilter.h"

namespace base
{
	namespace stream
	{
		StreamDecoderFilter::StreamDecoderFilter() : AbstractFilter()
		{}

		StreamDecoderFilter::~StreamDecoderFilter()
		{
			if (abstractDecoderPtr)
			{
				abstractDecoderPtr->destroyDecoder();
			}
		}

		int StreamDecoderFilter::inputData(StreamPacketPtr pkt)
		{
			int e{ pkt ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				if (!abstractDecoderPtr)
				{
					createNewDecoder(pkt->getStreamPacketType());
				}

				e = abstractDecoderPtr ? abstractDecoderPtr->inputData(pkt) : eBadOperate;
			}

			return e;
		}

		int StreamDecoderFilter::createNewPin()
		{
			const FilterType ft{ AbstractFilter::getFilterType() };

			if (FilterType::FILTER_TYPE_SOURCE == ft || FilterType::FILTER_TYPE_MEDIUM == ft)
			{
				AbstractPinPtr outputPin{
					boost::make_shared<AbstractPin>(
						boost::enable_shared_from_this<AbstractFilter>::shared_from_this(),
						PinType::PIN_TYPE_OUTPUT) };

				if (outputPin)
				{
					abstractPinGroup.insert(StreamDecoderFilterOutputPinName, outputPin);
				}
			}

			if (FilterType::FILTER_TYPE_TARGET == ft || FilterType::FILTER_TYPE_MEDIUM == ft)
			{
				AbstractPinPtr inputPin{
					boost::make_shared<AbstractPin>(
						boost::enable_shared_from_this<AbstractFilter>::shared_from_this(),
						PinType::PIN_TYPE_INPUT) };

				if (inputPin)
				{
					abstractPinGroup.insert(StreamDecoderFilterInputPinName, inputPin);
				}
			}

			return 0 < abstractPinGroup.size() ? eSuccess : eBadNewObject;
		}

		int StreamDecoderFilter::afterProcessInputDataNotification(StreamPacketPtr pkt)
		{
			int e{ pkt ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				AbstractPinPtr outputPin{ 
					abstractPinGroup.at(StreamDecoderFilterOutputPinName) };

				if (outputPin)
				{
					e = outputPin->inputData(pkt);
				}
				else
				{
					e = eObjectNotExist;
				}
			}

			return e;
		}

		int StreamDecoderFilter::createNewDecoder(
			const base::packet::StreamPacketType type /* = base::packet::StreamPacketType::STREAM_PACKET_TYPE_NONE */)
		{
			if (base::packet::StreamPacketType::STREAM_PACKET_TYPE_PRIVATE_HK == type)
			{
				abstractDecoderPtr = boost::make_shared<HikvisionSDKDecoder>(*this);
			}
			else if (base::packet::StreamPacketType::STREAM_PACKET_TYPE_PRIVATE_DH == type)
			{
			}
			else if (base::packet::StreamPacketType::STREAM_PACKET_TYPE_H264 == type)
			{
			}
			else if (base::packet::StreamPacketType::STREAM_PACKET_TYPE_AAC == type)
			{
			}

			return abstractDecoderPtr ? abstractDecoderPtr->createNewDecoder() : eBadNewObject;
		}
	}//namespace stream
}//namespace base
