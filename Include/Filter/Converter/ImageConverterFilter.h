//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					图片转换器类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
//

#ifndef BASE_STREAM_IMAGE_CONVERTER_FILTER_H
#define BASE_STREAM_IMAGE_CONVERTER_FILTER_H

#include "boost/shared_ptr.hpp"
#include "Filter/AbstractFilter.h"

namespace base
{
	namespace stream
	{
		BOOST_STATIC_CONSTANT(std::string, ImageConverterFilterInputPinName = "ImageConverterFilterInputPin");
		BOOST_STATIC_CONSTANT(std::string, ImageConverterFilterOutputPinName = "ImageConverterFilterOutputPin");

		class AbstractImageConverter;
		using AbstractImageConverterPtr = boost::shared_ptr<AbstractImageConverter>;

		class ImageConverterFilter : public AbstractFilter
		{
		public:
			ImageConverterFilter(void);
			virtual ~ImageConverterFilter(void);

		protected:
			int inputData(StreamPacketPtr pkt) override;
			int createNewPin(void) override;
			int afterProcessInputDataNotification(StreamPacketPtr pkt) override;

		private:
			//创建转换器实例
			//@type : 数据帧类型
			//@Return : 错误码
			//@Comment : 接收到第一个流包数据后再创建转换器实例
			int createNewConverter(
				const base::packet::StreamFrameType type = base::packet::StreamFrameType::STREAM_FRAME_TYPE_NONE);

		private:
			AbstractImageConverterPtr abstractImageConverterPtr;
		};//class ImageConverterFilter
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_IMAGE_CONVERTER_FILTER_H
