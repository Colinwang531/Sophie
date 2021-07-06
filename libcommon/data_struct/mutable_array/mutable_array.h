//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-07
//		Description :					可变数组
//
//		History:						Author									Date										Description
//										王科威									 2020-07-07									 创建
//

#ifndef FRAMEWORK_LIBCOMMON_MUTABLE_ARRAY_H
#define FRAMEWORK_LIBCOMMON_MUTABLE_ARRAY_H

#include <vector>
#include "libcommon/lock/rwlock.h"
using WRLock = framework::libcommon::WRLock;

template <class T>
class MutableArray
{
public:
	MutableArray(void) 
	{}
	~MutableArray(void) 
	{
		clear();
	}

public:
	void addItem(const T e)
	{
		wrlock.wlock();
		queue.push_back(e);
		wrlock.unwlock();
	}

	void removeItem(const unsigned long long index = 0)
	{
		int i{0};

		wrlock.wlock();
		for(typename std::vector<T>::iterator it = queue.begin(); it != queue.end();)
		{
			if(index == i)
			{
				queue.erase(it);
				break;
			}

			++i;
			++it;
		}
		wrlock.unwlock();
	}

	void clear(void)
	{
		wrlock.wlock();
		queue.clear();
		wrlock.unwlock();
	}
	
	T item(const unsigned long long index = 0)
	{
		T e{};

		if (index < queue.size())
		{
			e = queue[index];
		}

		return e;
	}

	T* itemRef(const unsigned long long index = 0)
	{
		T* e{nullptr};

		if (index < queue.size())
		{
			e = &queue[index];
		}

		return e;
	}

	const unsigned long long size(void) const
	{
		return queue.size();
	}

private:
	std::vector<T> queue;
	WRLock wrlock;
};//class MutableArray

#endif//FRAMEWORK_LIBCOMMON_MUTABLE_ARRAY_H

