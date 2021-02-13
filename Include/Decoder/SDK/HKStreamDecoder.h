//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					海康流SDK解码器类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
//

#ifndef FRAMEWORK_MULTIMEDIA_HK_STREAM_DECODER_H
#define FRAMEWORK_MULTIMEDIA_HK_STREAM_DECODER_H

#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include "PlayCtrl_Linux64_V7.3.3.30_Build20171214/LinuxPlayM4.h"
#include "DataStruct/FIFOQueue.h"
#include "Data/Data.h"
using Data = framework::data::Data;
using DataPtr = boost::shared_ptr<Data>;

namespace framework
{
	namespace multimedia
	{
		typedef boost::function<void(DataPtr)> DataNotificationCallback;

		class HKStreamDecoder
		{
		public:
			HKStreamDecoder(
				DataNotificationCallback callback = nullptr);
			virtual ~HKStreamDecoder(void);

		public:
			int inputData(DataPtr data);

		private:
			static void CALLBACK afterDecodeOneFrameCallback(
				int nPort, char* pBuf, int nSize, FRAME_INFO* pFrameInfo, void* nUser, int nReserved2);
			void decodeWorkerThread(void);

		private:
			bool stopped;
			int decoderID;
			FIFOQueue<DataPtr> fifo;
			DataNotificationCallback dataNotificationCallback;
			std::string extendID;
		};//class HikvisionSDKDecoder
	}//namespace multimedia
}//namespace framework

#endif//FRAMEWORK_MULTIMEDIA_HK_STREAM_DECODER_H
