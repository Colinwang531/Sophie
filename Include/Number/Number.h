//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-15
//		Description :					������
//
//		History:						Author									Date										Description
//										������									2020-05-26									����
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
			//��ȡ��������
			//@Return : ��������
			const long long getOddNumber(void);

		private:
			long long number;
		};//class Time
	}//namespace number
}//namespace base

#endif//TIME_H
