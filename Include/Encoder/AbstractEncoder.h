//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					图片编码类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
//

#ifndef BASE_STREAM_ABSTRACT_ENCODE_H
#define BASE_STREAM_ABSTRACT_ENCODE_H

#include "boost/shared_ptr.hpp"
#include "Packet/Stream/StreamPacket.h"
using StreamPacket = base::packet::StreamPacket;
using StreamPacketPtr = boost::shared_ptr<StreamPacket>;

namespace base
{
	namespace stream
	{
		class AbstractFilter;

		class AbstractEncoder
		{
		public:
			AbstractEncoder(AbstractFilter& filter);
			virtual ~AbstractEncoder(void);

		public:
			//输入流数据包
			//@pkt : 流数据包
			//@Return : 错误码
			virtual int inputData(StreamPacketPtr pkt) = 0;

		protected:
			AbstractFilter& abstractFilter;
		};//class AbstractEncoder
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_ABSTRACT_ENCODE_H
