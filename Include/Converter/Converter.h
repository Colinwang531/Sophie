//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					��ʽת����
//
//		History:						Author									Date										Description
//										������									2020-09-23									����
//

#ifndef FRAMEWORK_MULTIMEDIA_CONVERTER_H
#define FRAMEWORK_MULTIMEDIA_CONVERTER_H

#include "boost/shared_ptr.hpp"
#include "Data/Data.h"
using Data = framework::data::Data;
using DataPtr = boost::shared_ptr<Data>;

namespace framework
{
	namespace multimedia
	{
		class Converter
		{
		public:
			Converter(void);
			virtual ~Converter(void);

		public:
			//���������ݰ�
			//@pkt : �����ݰ�
			//@Return : ������
			int inputData(DataPtr data);

		private:
			void* cvter;
		};//class Converter
	}//namespace multimedia
}//namespace framework

#endif//FRAMEWORK_MULTIMEDIA_CONVERTER_H
