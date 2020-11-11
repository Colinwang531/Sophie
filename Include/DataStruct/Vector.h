//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-06-11
//		Description :					可变数组模板
//
//		History:						Author									Date										Description
//										王科威									2020-06-11									创建
//

#ifndef VECTOR_H
#define VECTOR_H

#include <vector>
#include "Mutex/RWLock.h"

template <class T>
class Vector
{
public:
	Vector(void) {}
	virtual ~Vector(void) 
	{
		clear();
	}

public:
	void pushBack(const T e)
	{
		WriteLock wl{ mtx };
		queue.push_back(e);
	}

	void removeFront(void)
	{
		WriteLock wl{ mtx };
		if (0 < queue.size())
		{
			queue.erase(queue.begin());
		}
	}

	T popFront(void)
	{
		T t{};

		WriteLock wl{ mtx };
		if (0 < queue.size())
		{
			t = at(0);
			queue.erase(queue.begin());
		}

		return t;
	}

	void clear(void)
	{
		WriteLock wl{ mtx };
		queue.clear();
	}
	
	T at(const unsigned long long index = 0)
	{
		T e{};

//		ReadLock rl{ mtx };
		if (index < queue.size())
		{
			e = queue[index];
		}

		return e;
	}

	unsigned long long size(void)
	{
//		ReadLock rl{ mtx };
		return queue.size();
	}

private:
	std::vector<T> queue;
	SharedMutex mtx;
};//class FIFOQueue

#endif//VECTOR_H

