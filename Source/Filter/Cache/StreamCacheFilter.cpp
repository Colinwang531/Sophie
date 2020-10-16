#include "boost/enable_shared_from_this.hpp"
#include "boost/make_shared.hpp"
#include "Error.h"
#include "Pin/AbstractPin.h"
#include "Filter/Cache/StreamCacheFilter.h"

namespace base
{
	namespace stream
	{
		StreamCacheFilter::StreamCacheFilter(const int size /* = eMaxCacheSize */)
			: AbstractFilter(FilterType::FILTER_TYPE_SOURCE), 
			streamCacheData{ new(std::nothrow) unsigned char[size] }, streamCacheDataBytes{ 0 }
		{}

		StreamCacheFilter::~StreamCacheFilter()
		{
			boost::checked_array_delete(streamCacheData);
		}

		int StreamCacheFilter::createNewPin()
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
					abstractPinGroup.insert(StreamCacheFilterOutputPinName, outputPin);
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
					abstractPinGroup.insert(StreamCacheFilterInputPinName, inputPin);
				}
			}

			return 0 < abstractPinGroup.size() ? eSuccess : eBadNewObject;
		}

		int StreamCacheFilter::inputData(StreamPacketPtr pkt)
		{
			int e{ streamCacheData && pkt ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				const unsigned char* data{ pkt->getStreamData() };
				const int bytes{ pkt->getStreamDataBytes() };
				int more{ 0 }, part{ 0 };

				if (0 == streamCacheDataBytes)
				{
					const int* headFlag{ (const int*)data };
					if (base::packet::StreamHeadSize::eFixedHeadFlag != *headFlag)
					{
						return eBadOperate;
					}
				}

				if (streamCacheDataBytes >= eMaxCacheSize)
				{
					//Amazing
					//没办法只有丢数据了
					streamCacheDataBytes = 0;
					return eOutOfRange;
				}

				if (bytes + streamCacheDataBytes > eMaxCacheSize)
				{
					//将部分数据拷贝进缓存进行处理
					//剩余部分必须在处理结束后继续拷贝进缓存
					part = eMaxCacheSize - streamCacheDataBytes;
					more = bytes - part;

#ifdef WINDOWS
					memcpy_s(streamCacheData + streamCacheDataBytes, part, data, part);
#else
					memcpy(streamCacheData + streamCacheDataBytes, data, part);
#endif//WINDOWS
					streamCacheDataBytes += part;
				}
				else
				{
#ifdef WINDOWS
					memcpy_s(streamCacheData + streamCacheDataBytes, bytes, data, bytes);
#else
					memcpy(streamCacheData + streamCacheDataBytes, data, bytes);
#endif//WINDOWS
					streamCacheDataBytes += bytes;
				}

				//一次解析一帧或多帧数据
				e = parseOneFrameData();
				//所有帧解析正确才能拷贝剩余数据进缓存
				if (eSuccess == e && e < more && 0 < part)
				{
#ifdef WINDOWS
					memcpy_s(streamCacheData + streamCacheDataBytes, more, data + part, more);
#else
					memcpy(streamCacheData + streamCacheDataBytes, data + part, more);
#endif//WINDOWS
					streamCacheDataBytes += more;
				}
			}

			return e;
		}

		int StreamCacheFilter::afterProcessInputDataNotification(StreamPacketPtr pkt)
		{
			int e{ pkt ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				AbstractPinPtr outputPin{
					abstractPinGroup.at(StreamCacheFilterOutputPinName) };

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

		int StreamCacheFilter::parseOneFrameData()
		{
			int pos{ 0 }, e{ eSuccess };

			while (pos < streamCacheDataBytes)
			{
				//剩余数据小于固定头长度就移动剩余数据到缓存区域起始位置
				if (streamCacheDataBytes - pos <= base::packet::StreamHeadSize::eFixedHeadSize)
				{
					streamCacheDataBytes -= pos;
#ifdef WINDOWS
					memcpy_s(streamCacheData, streamCacheDataBytes, streamCacheData + pos, streamCacheDataBytes);
#else
					memcpy(streamCacheData, streamCacheData + pos, streamCacheDataBytes);
#endif//WINDOWS
					return e;
				}

				//检查每一帧头数据是否正确
				const int* headFlag{ (const int*)(streamCacheData + pos) };
				if (base::packet::StreamHeadSize::eFixedHeadFlag != *headFlag)
				{
					//Ops!!!!!!!!!!!!!!!!!!!!!!!!!
					streamCacheDataBytes = 0;
					return eBadOperate;
				}

				const int* dataType{ (const int*)(streamCacheData + pos + 4) };
				const int* streamType{ (const int*)(streamCacheData + pos + 8) };
				const int* frameType{ (const int*)(streamCacheData + pos + 12) };
				const int* frameSize{ (const int*)(streamCacheData + pos + 16) };
				const long long* frameSequence{ (const long long*)(streamCacheData + pos + 20) };
				const long long* frameTimestamp{ (const long long*)(streamCacheData + pos + 28) };

				//剩余数据小于实际数据长度就移动剩余数据到缓存区域起始位置
				if (streamCacheDataBytes - pos - base::packet::StreamHeadSize::eFixedHeadSize < *frameSize)
				{
					streamCacheDataBytes -= pos;
#ifdef WINDOWS
					memcpy_s(streamCacheData, streamCacheDataBytes, streamCacheData + pos, streamCacheDataBytes);
#else
					memcpy(streamCacheData, streamCacheData + pos, streamCacheDataBytes);
#endif//WINDOWS
					return e;
				}

				StreamPacketPtr pkt{
					boost::make_shared<StreamPacket>(
						static_cast<base::packet::StreamPacketType>(*streamType),
						static_cast<base::packet::StreamFrameType>(*frameType)) };

				if (pkt)
				{
					pkt->setPacketSequence(*frameSequence);
					pkt->setPacketTimestamp(*frameTimestamp);
					//从此开始传递的包数据不包含流数据头内容
					pkt->setStreamData(streamCacheData + pos + base::packet::StreamHeadSize::eFixedHeadSize, *frameSize);
					afterProcessInputDataNotification(pkt);
				}

				pos += (base::packet::StreamHeadSize::eFixedHeadSize + *frameSize);
			}

			streamCacheDataBytes -= pos;
			return e;
		}
	}//namespace stream
}//namespace base
