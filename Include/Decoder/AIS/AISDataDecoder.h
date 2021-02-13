//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					AIS解码器类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
//

#ifndef FRAMEWORK_MULTIMEDIA_AIS_DECODER_H
#define FRAMEWORK_MULTIMEDIA_AIS_DECODER_H

#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include "Data/Data.h"
using Data = framework::data::Data;
using DataPtr = boost::shared_ptr<Data>;

namespace framework
{
	namespace multimedia
	{
		typedef boost::function<void(DataPtr)> DataNotificationCallback;

		class AISDataDecoder
		{
		public:
			AISDataDecoder(
				DataNotificationCallback callback = nullptr);
			~AISDataDecoder(void);

		public:
			int inputData(DataPtr data);

		private:
			void parseMessage_1_2_3(const char* msg = nullptr);
			void parseMessage5(const char* msg = nullptr);
			void parseMessage9(const char* msg = nullptr);
			void parseMessage18(const char* msg = nullptr);
			void parseMessage19(const char* msg = nullptr);
			void parseMessage21(const char* msg = nullptr);

		private:
			DataNotificationCallback dataNotificationCallback;
		};//class AISDataDecoder
	}//namespace multimedia
}//namespace framework

#endif//FRAMEWORK_MULTIMEDIA_AIS_DECODER_H
