//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-23
//		Description :					UUID生成器类
//
//		History:						Author									Date										Description
//										王科威									 2021-06-23									 创建
//

#ifndef FRAMEWORK_LIBCOMMON_UUID_H
#define FRAMEWORK_LIBCOMMON_UUID_H

#include <string>

namespace framework
{
	namespace libcommon
	{
		class Uuid
		{
		public:
			Uuid(void);
			~Uuid(void);

		public:
			//随机生成UUID
			//@Return : UUID
			const std::string generateRandomUuid(void);
		};//class Uuid
	}//namespace libcommon
}//namespace framework

#endif//FRAMEWORK_LIBCOMMON_UUID_H
