//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-07
//		Description :					FIFO队列类
//
//		History:						Author									Date										Description
//										王科威									2020-07-07									创建
//

#ifndef FIFO_QUEUE_H
#define FIFO_QUEUE_H

#include <deque>
#include "Mutex/RWLock.h"

template <class T>
class FIFOQueue
{
public:
	FIFOQueue(
		const unsigned long long cap = 0xFFFF) 
		: capacity{ cap }
	{}
	~FIFOQueue(void)
	{}

public:
	void pushBack(const T e)
	{
		const unsigned long long itemNumber{ queue.size() };
		if (capacity > itemNumber)
		{
			WriteLock wl{ mtx };
			queue.push_back(e);
		}
	}

	void popFront(void)
	{
		WriteLock wl{ mtx };
		typename std::deque<T>::iterator it{ queue.begin() };
		if (queue.end() != it)
		{
			queue.erase(it);
		}
	}
	
	T front(void)
	{
		T e{};

		const unsigned long long itemNumber{ queue.size() };
		if (0 < queue.size())
		{
			ReadLock rl{ mtx };
			e = queue[0];
		}

		return e;
	}

	const unsigned long long size(void)
	{
		ReadLock rl{ mtx };
		return queue.size();
	}

	void clear()
	{
		WriteLock wl{ mtx };
		queue.clear();
	}

private:
	unsigned long long capacity;
	std::deque<T> queue;
	SharedMutex mtx;
};//class FIFOQueue

#endif//FIFO_QUEUE_H
