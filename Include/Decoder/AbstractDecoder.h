//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					�����������
//
//		History:						Author									Date										Description
//										������									2020-09-23									����
//

#ifndef BASE_STREAM_ABSTRACT_DECODER_H
#define BASE_STREAM_ABSTRACT_DECODER_H

#include "boost/shared_ptr.hpp"
#include "Packet/Stream/StreamPacket.h"
using StreamPacket = base::packet::StreamPacket;
using StreamPacketPtr = boost::shared_ptr<StreamPacket>;

namespace base
{
	namespace stream
	{
		class AbstractFilter;

		class AbstractDecoder
		{
		public:
			AbstractDecoder(AbstractFilter& filter);
			virtual ~AbstractDecoder(void);

		public:
			//���������ٽ�����ʵ���ڲ�����Ҫ����������
			//@Return : ������
			virtual int createNewDecoder(void);
			virtual int destroyDecoder(void);

			//���������ݰ�
			//@pkt : �����ݰ�
			//@Return : ������
			virtual int inputData(StreamPacketPtr pkt) = 0;

		protected:
			//���ݽ�������
			virtual void processingDecodeData(void) = 0;

		private:
			//���빤���߳�
			void processDecodeWorkerkThread(void);

		protected:
			bool stopped;
			AbstractFilter& abstractFilter;
		};//class AbstractDecoder
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_ABSTRACT_DECODER_H
