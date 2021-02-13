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
//										王科威									2020-11-25									修改时间戳返回值为无符号类型
//

#ifndef FRAMEWORK_TIME_TIME_H
#define FRAMEWORK_TIME_TIME_H

namespace framework
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
			const unsigned long long tickcount(void);

			//执行睡眠
			//@interval : 睡眠时间,以秒为单位
			void sleep(const unsigned int interval = 1);
		};//class Time
	}//namespace time
}//namespace framework

#endif//FRAMEWORK_TIME_TIME_H
