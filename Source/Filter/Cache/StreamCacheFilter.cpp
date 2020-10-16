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
					//û�취ֻ�ж�������
					streamCacheDataBytes = 0;
					return eOutOfRange;
				}

				if (bytes + streamCacheDataBytes > eMaxCacheSize)
				{
					//���������ݿ�����������д���
					//ʣ�ಿ�ֱ����ڴ���������������������
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

				//һ�ν���һ֡���֡����
				e = parseOneFrameData();
				//����֡������ȷ���ܿ���ʣ�����ݽ�����
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
				//ʣ������С�ڹ̶�ͷ���Ⱦ��ƶ�ʣ�����ݵ�����������ʼλ��
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

				//���ÿһ֡ͷ�����Ƿ���ȷ
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

				//ʣ������С��ʵ�����ݳ��Ⱦ��ƶ�ʣ�����ݵ�����������ʼλ��
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
					//�Ӵ˿�ʼ���ݵİ����ݲ�����������ͷ����
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
