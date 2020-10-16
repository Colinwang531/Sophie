//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					��������
//
//		History:						Author									Date										Description
//										������									2020-09-23									����
//

#ifndef BASE_StREAM_STREAM_CACHE_FILTER_H
#define BASE_StREAM_STREAM_CACHE_FILTER_H

#include "Filter/AbstractFilter.h"

namespace base
{
	namespace stream
	{
		BOOST_STATIC_CONSTANT(std::string, StreamCacheFilterInputPinName = "StreamCacheFilterInputPin");
		BOOST_STATIC_CONSTANT(std::string, StreamCacheFilterOutputPinName = "StreamCacheFilterOutputPin");

		class StreamCacheFilter : public AbstractFilter
		{
		public:
			StreamCacheFilter(
				const int size = eMaxCacheSize);
			virtual ~StreamCacheFilter(void);

		protected:
			int createNewPin(void) override;
			int inputData(StreamPacketPtr pkt) override;
			int afterProcessInputDataNotification(StreamPacketPtr pkt) override;

		private:
			//����������һ֡����
			//@Return : ������
			int parseOneFrameData(void);

		private:
			enum
			{
				eMaxCacheSize = 3 * 1024 * 1024
			};
			unsigned char* streamCacheData;
			int streamCacheDataBytes;
		};//class StreamCacheFilter
	}//namespace stream
}//namespace base

#endif//BASE_StREAM_STREAM_CACHE_FILTER_H
