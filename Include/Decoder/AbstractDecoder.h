//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					抽象解码器类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
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
			//创建和销毁解码器实例内部所需要的其他东西
			//@Return : 错误码
			virtual int createNewDecoder(void);
			virtual int destroyDecoder(void);

			//输入流数据包
			//@pkt : 流数据包
			//@Return : 错误码
			virtual int inputData(StreamPacketPtr pkt) = 0;

		protected:
			//数据解码流程
			virtual void processingDecodeData(void) = 0;

		private:
			//解码工作线程
			void processDecodeWorkerkThread(void);

		protected:
			bool stopped;
			AbstractFilter& abstractFilter;
		};//class AbstractDecoder
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_ABSTRACT_DECODER_H
