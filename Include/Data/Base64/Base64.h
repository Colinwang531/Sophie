//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-17
//		Description :					Base64数据类
//
//		History:						Author									Date										Description
//										王科威									2020-09-17									创建
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
			//Base64编码
			//@data : 帧数据
			//@bytes : 帧数据大小
			//@Return : 错误码
			int encodeBase64(
				const char* data = nullptr,
				const int bytes = 0);
		};//class Base64
	}//namespace data
}//namespace framework

#endif//FRAMEWORK_DATA_FRAME_H
