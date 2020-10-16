//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					Image��ʽת����
//
//		History:						Author									Date										Description
//										������									2020-09-23									����
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
			//���������ݰ�
			//@pkt : �����ݰ�
			//@Return : ������
			virtual int inputData(StreamPacketPtr pkt) = 0;

		protected:
			AbstractFilter& abstractFilter;
		};//class MediaConverter
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_ABSTRACT_IMAGE_CONVERTER_H
