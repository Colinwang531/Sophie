//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					流解码器类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
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
			//创建解码器实例
			//@type : 数据包类型
			//@Return : 错误码
			//@Comment : 接收到第一个流包数据后再创建解码器实例
			int createNewDecoder(
				const base::packet::StreamPacketType type = base::packet::StreamPacketType::STREAM_PACKET_TYPE_NONE);

		private:
			AbstractDecoderPtr abstractDecoderPtr;
		};//class StreamDecoderFilter
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_STREAM_DECODER_FILTER_H
