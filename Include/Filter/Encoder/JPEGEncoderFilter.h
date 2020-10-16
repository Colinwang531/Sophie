//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					图片编码器类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
//

#ifndef BASE_STREAM_JPEG_ENCODER_FILTER_H
#define BASE_STREAM_JPEG_ENCODER_FILTER_H

#include "boost/shared_ptr.hpp"
#include "Filter/AbstractFilter.h"

namespace base
{
	namespace stream
	{
		BOOST_STATIC_CONSTANT(std::string, JPEGImageEncoderFilterInputPinName = "JPEGImageEncoderFilterInputPin");
		BOOST_STATIC_CONSTANT(std::string, JPEGImageEncoderFilterOutputPinName = "JPEGImageEncoderFilterOutputPin");

		class AbstractEncoder;
		using AbstractEncoderPtr = boost::shared_ptr<AbstractEncoder>;

		class JPEGEncoderFilter : public AbstractFilter
		{
		public:
			JPEGEncoderFilter(void);
			virtual ~JPEGEncoderFilter(void);

		protected:
			int inputData(StreamPacketPtr pkt) override;
			int createNewPin(void) override;
			int afterProcessInputDataNotification(StreamPacketPtr pkt) override;

		private:
			//创建编码器实例
			//@Return : 错误码
			//@Comment : 接收到第一个流包数据后再创建转换器实例
			int createNewEncoder(void);

		private:
			AbstractEncoderPtr abstractEncoderPtr;
		};//class JPEGEncoderFilter
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_JPEG_ENCODER_FILTER_H
