//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-17
//		Description :					Base64������
//
//		History:						Author									Date										Description
//										������									2020-09-17									����
//

#ifndef FRAMEWORK_DATA_BASE64_H
#define FRAMEWORK_DATA_BASE64_H

#include "Data/Data.h"

namespace framework
{
	namespace data
	{
		class Base64 final : public Data
		{
		public:
			Base64(void);
			~Base64(void);

		public:
			//Base64����
			//@data : ֡����
			//@bytes : ֡���ݴ�С
			//@Return : ������
			int encodeBase64(
				const char* data = nullptr,
				const int bytes = 0);
		};//class Base64
	}//namespace data
}//namespace framework

#endif//FRAMEWORK_DATA_FRAME_H
