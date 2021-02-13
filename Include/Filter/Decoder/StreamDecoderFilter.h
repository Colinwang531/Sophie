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

#ifndef FRAMEWORK_MULTIMEDIA_STREAM_DECODER_FILTER_H
#define FRAMEWORK_MULTIMEDIA_STREAM_DECODER_FILTER_H

#include "Decoder/Decoder.h"
#include "Filter/Filter.h"

namespace framework
{
	namespace multimedia
	{
		class StreamDecoderFilter 
			: public Filter, protected Decoder
		{
		public:
			StreamDecoderFilter(void);
			virtual ~StreamDecoderFilter(void);

		protected:
			int inputData(DataPtr data) override;
			void afterDataDecodeNotification(DataPtr data) override;
		};//class StreamDecoderFilter
	}//namespace multimedia
}//namespace framework

#endif//FRAMEWORK_MULTIMEDIA_STREAM_DECODER_FILTER_H
