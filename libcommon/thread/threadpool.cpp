#include "boost/thread.hpp"
#include "libcommon/error.h"
#include "threadpool.h"

namespace framework
{
	namespace libcommon
	{
		static boost::thread_group pool;

		ThreadPool::ThreadPool()
		{}
		ThreadPool::~ThreadPool()
		{}

		void* ThreadPool::create(Function f)
		{
			return pool.create_thread(f);
		}

		int ThreadPool::destroy(void* t /* = nullptr */)
		{
			CommonError e{
				t ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER};

			if (CommonError::COMMON_ERROR_SUCCESS == e)
			{
				boost::thread* t_{ reinterpret_cast<boost::thread*>(t) };

				if (pool.is_thread_in(t_))
				{
					pool.remove_thread(t_);
				}
				else
				{
					e = CommonError::COMMON_ERROR_NOT_EXIST;
				}
			}

			return (int)e;
		}
	}//namespace libcommon
}//namespace framework
