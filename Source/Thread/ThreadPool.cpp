#include "Thread/ThreadPool.h"

namespace framework
{
	namespace thread
	{
		ThreadPool::ThreadPool(){}
		ThreadPool::~ThreadPool(){}

		void* ThreadPool::createNewThread(Function f)
		{
			return tg.create_thread(f);
		}

		void ThreadPool::destroyThread(void* t /* = nullptr */)
		{
			boost::thread* thr{ reinterpret_cast<boost::thread*>(t) };

			if (thr && tg.is_thread_in(thr))
			{
				tg.remove_thread(thr);
			}
		}

		void ThreadPool::shutdownAllThread()
		{
			tg.join_all();
		}
	}//namespace thread
}//namespace framework
