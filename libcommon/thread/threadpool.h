//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-05
//		Description :					线程池类
//
//		History:						Author									Date										Description
//										王科威									2021-06-05									创建
//

#ifndef FRAMEWORK_LIBCOMMON_THREAD_POOL_H
#define FRAMEWORK_LIBCOMMON_THREAD_POOL_H

#include <functional>

namespace framework
{
	namespace libcommon
	{
		typedef std::function<void(void)> Function;

		class ThreadPool
		{
		public:
			ThreadPool(void);
			~ThreadPool(void);

		public:
			//创建线程
			//@f : 线程函数
			//@Return : 线程句柄值
			//@Comment : 仅支持不带参数的线程函数
			void* create(Function f);

			//销毁线程
			//@tid : 线程句柄值
			//@Return : 错误码
			int destroy(void* tid = nullptr);
		};//class ThreadPool
	}//namespace libcommon
}//namespace framework

#endif//FRAMEWORK_LIBCOMMON_THREAD_POOL_H
