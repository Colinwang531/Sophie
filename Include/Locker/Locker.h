// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Read and write locker definitions.
//

#ifndef BASE_LOCKER_H
#define BASE_LOCKER_H

#include "boost/thread/locks.hpp"
#include "boost/thread/shared_mutex.hpp"

namespace base
{
	namespace locker
	{
		using SharedMutex = boost::shared_mutex;
		using WriteLocker = boost::unique_lock<SharedMutex>;
		using ReadLocker = boost::shared_lock<SharedMutex>;
	}//namespace locker
}//namespace base

#endif//BASE_LOCKER_H

