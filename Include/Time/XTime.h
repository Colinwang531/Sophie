//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-15
//		Description :					时间类
//
//		History:						Author									Date										Description
//										王科威									2020-05-26									创建
//

#ifndef TIME_H
#define TIME_H

namespace base
{
	namespace time
	{
		class Time
		{
		public:
			Time(void);
			virtual ~Time(void);

		public:
			//获取系统时间戳,起始时间为1970-1-1 0：0：0
			//@Return : 以毫秒为单位的时间长度
			const long long tickcount(void);
		};//class Time
	}//namespace time
}//namespace base

#endif//TIME_H
