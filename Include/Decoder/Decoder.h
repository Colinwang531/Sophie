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

#ifndef FRAMEWORK_MULTIMEDIA_DECODER_H
#define FRAMEWORK_MULTIMEDIA_DECODER_H

#include "boost/shared_ptr.hpp"
#include "Data/Data.h"
using Data = framework::data::Data;
using DataPtr = boost::shared_ptr<Data>;

namespace framework
{
	namespace multimedia
	{
		class Decoder
		{
		public:
			Decoder(void);
			virtual ~Decoder(void);

		public:
			//��������
			//@data : ����
			//@Return : ������
			int inputData(DataPtr data);

		protected:
			//��������֪ͨ
			//@data : ����
			virtual void afterDataDecodeNotification(DataPtr data) = 0;

		private:
			//�������ݻص�
			//@data : ����
			void afterDataDecodeCallback(DataPtr data);

		private:
			void* decoder;
		};//class Decoder
	}//namespace multimedia
}//namespace framework

#endif//FRAMEWORK_MULTIMEDIA_DECODER_H
