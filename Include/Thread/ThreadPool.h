//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-06-02
//		Description :					线程池类
//
//		History:						Author									Date										Description
//										王科威									2020-06-02									创建
//

#ifndef FRAMEWORK_THREAD_THREAD_POOL_H
#define FRAMEWORK_THREAD_THREAD_POOL_H

#include "boost/function.hpp"
#include "boost/serialization/singleton.hpp"
#include "boost/thread.hpp"

namespace framework
{
	namespace thread
	{
		typedef boost::function<void(void)> Function;

		class ThreadPool :
			public boost::serialization::singleton<ThreadPool>
		{
		public:
			ThreadPool(void);
			virtual ~ThreadPool(void);

		public:
			//创建新线程实例
			//@f : 线程函数
			//@Return : 线程句柄值
			void* createNewThread(Function f);

			//销毁线程
			//@t : 线程句柄值
			void destroyThread(void* t = nullptr);

			//线程安全退出
			void shutdownAllThread(void);

		private:
			boost::thread_group tg;
		};//class ThreadPool
	}//namespace thread
}//namespace framework

#endif//FRAMEWORK_THREAD_THREAD_POOL_H
