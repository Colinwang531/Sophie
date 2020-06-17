//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-15
//		Description :					数字类
//
//		History:						Author									Date										Description
//										王科威									2020-05-26									创建
//

#ifndef NUMBER_H
#define NUMBER_H

#include "boost/serialization/singleton.hpp"

namespace base
{
	namespace number
	{
		class OddNumber : 
			public boost::serialization::singleton<OddNumber>
		{
		public:
			OddNumber(void);
			~OddNumber(void);

		public:
			//获取奇数数字
			//@Return : 奇数数字
			const long long getOddNumber(void);

		private:
			long long number;
		};//class Time
	}//namespace number
}//namespace base

#endif//TIME_H
