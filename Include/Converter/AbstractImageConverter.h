//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					Image格式转换类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
//

#ifndef BASE_STREAM_ABSTRACT_IMAGE_CONVERTER_H
#define BASE_STREAM_ABSTRACT_IMAGE_CONVERTER_H

#include "boost/shared_ptr.hpp"
#include "Packet/Stream/StreamPacket.h"
using StreamPacket = base::packet::StreamPacket;
using StreamPacketPtr = boost::shared_ptr<StreamPacket>;

namespace base
{
	namespace stream
	{
		class AbstractFilter;

		class AbstractImageConverter
		{
		public:
			AbstractImageConverter(AbstractFilter& filter);
			virtual ~AbstractImageConverter(void);

		public:
			//输入流数据包
			//@pkt : 流数据包
			//@Return : 错误码
			virtual int inputData(StreamPacketPtr pkt) = 0;

		protected:
			AbstractFilter& abstractFilter;
		};//class MediaConverter
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_ABSTRACT_IMAGE_CONVERTER_H
