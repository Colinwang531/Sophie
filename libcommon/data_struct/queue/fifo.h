//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-07
//		Description :					FIFO数据结构
//
//		History:						Author									Date										Description
//										王科威									 2020-07-07									 创建
//

#ifndef FRAMEWORK_LIBCOMMON_FIFO_QUEUE_H
#define FRAMEWORK_LIBCOMMON_FIFO_QUEUE_H

#include <deque>
#include "lock/smtx.h"
using SharedMutex = framework::libcommon::SharedMutex;
#include "error.h"

template <class T>
class Fifo
{
public:
	Fifo(const unsigned int max = 65535) : capacity{0 < max ? max : 65535}
	{}
	~Fifo(void)
	{
		clear();
	}

public:
	int add(const T elem)
	{
		CommonError e{CommonError::COMMON_ERROR_OUT_OF_RANGE};
		const unsigned int currentNumber{ elements.size() };

		if (capacity > currentNumber)
		{
			mtx.lockWrite();
			elements.push_back(elem);
			mtx.unlockWrite();
			e = CommonError::COMMON_ERROR_SUCCESS;
		}

		return static_cast<int>(e);
	}

	int remove(void)
	{
		CommonError e{CommonError::COMMON_ERROR_OUT_OF_RANGE};
		const unsigned int currentNumber{ elements.size() };

		if (0 < currentNumber)
		{
			mtx.lockWrite();
			elements.erase(elements.begin());
			mtx.unlockWrite();
			e = CommonError::COMMON_ERROR_SUCCESS;
		}

		return static_cast<int>(e);
	}
	
	T* first(void)
	{
		T* elem{nullptr};
		const unsigned int currentNumber{ elements.size() };

		if (0 < currentNumber)
		{
			mtx.lockWrite();
			elem = &elements[0];
			mtx.unlockWrite();
		}

		return elem;
	}

	const unsigned int size(void)
	{
		mtx.lockRead();
		const unsigned int number{queue.size()};
		mtx.unlockRead();

		return number;
	}

	void clear()
	{
		mtx.lockWrite();
		queue.clear();
		mtx.unlockWrite();
	}

private:
	const unsigned int capacity;
	std::deque<T> elements;
	SharedMutex mtx;
};//class Fifo

#endif//FRAMEWORK_LIBCOMMON_FIFO_QUEUE_H
