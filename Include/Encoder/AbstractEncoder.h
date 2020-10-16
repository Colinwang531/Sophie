//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					ͼƬ������
//
//		History:						Author									Date										Description
//										������									2020-09-23									����
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
			//���������ݰ�
			//@pkt : �����ݰ�
			//@Return : ������
			virtual int inputData(StreamPacketPtr pkt) = 0;

		protected:
			AbstractFilter& abstractFilter;
		};//class AbstractEncoder
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_ABSTRACT_ENCODE_H
