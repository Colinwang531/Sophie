//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					����������
//
//		History:						Author									Date										Description
//										������									2020-09-23									����
//

#ifndef BASE_STREAM_STREAM_DECODER_FILTER_H
#define BASE_STREAM_STREAM_DECODER_FILTER_H

#include "boost/shared_ptr.hpp"
#include "Filter/AbstractFilter.h"

namespace base
{
	namespace stream
	{
		BOOST_STATIC_CONSTANT(std::string, StreamDecoderFilterInputPinName = "StreamDecoderFilterInputPin");
		BOOST_STATIC_CONSTANT(std::string, StreamDecoderFilterOutputPinName = "StreamDecoderFilterOutputPin");

		class AbstractDecoder;
		using AbstractDecoderPtr = boost::shared_ptr<AbstractDecoder>;

		class StreamDecoderFilter : public AbstractFilter
		{
		public:
			StreamDecoderFilter(void);
			virtual ~StreamDecoderFilter(void);

		protected:
			int inputData(StreamPacketPtr pkt) override;
			int createNewPin(void) override;
			int afterProcessInputDataNotification(StreamPacketPtr pkt) override;

		private:
			//����������ʵ��
			//@type : ���ݰ�����
			//@Return : ������
			//@Comment : ���յ���һ���������ݺ��ٴ���������ʵ��
			int createNewDecoder(
				const base::packet::StreamPacketType type = base::packet::StreamPacketType::STREAM_PACKET_TYPE_NONE);

		private:
			AbstractDecoderPtr abstractDecoderPtr;
		};//class StreamDecoderFilter
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_STREAM_DECODER_FILTER_H
