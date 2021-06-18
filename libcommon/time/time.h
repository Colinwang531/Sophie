//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-15
//		Description :					时间类
//
//		History:						Author									Date										Description
//										王科威									 2021-06-15									 创建
//

#ifndef FRAMEWORK_LIBCOMMON_TIME_H
#define FRAMEWORK_LIBCOMMON_TIME_H

namespace framework
{
	namespace libcommon
	{
		class Time
		{
		public:
			Time(void);
			~Time(void);

		public:
			//获取系统时间戳,起始时间为1970-1-1 0:0:0
			//@Return : 时间长度,以毫秒为单位
			const unsigned long long tickcount(void);

			//执行睡眠
			//@interval : 时间长度,以秒为单位
			void sleep(const unsigned int interval = 1);
		};//class Time
	}//namespace libcommon
}//namespace framework

#endif//FRAMEWORK_LIBCOMMON_TIME_H
