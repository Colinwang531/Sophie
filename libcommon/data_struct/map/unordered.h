//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-07
//		Description :					无序映射数据结构
//
//		History:						Author									Date										Description
//										王科威									 2020-07-07									 创建
//

#ifndef FRAMEWORK_LIBCOMMON_UNORDERED_MAP_H
#define FRAMEWORK_LIBCOMMON_UNORDERED_MAP_H

#include <vector>
#include "boost/unordered_map.hpp"
#include "lock/smtx.h"
using SharedMutex = framework::libcommon::SharedMutex;
#include "error.h"

template <class Key, class Value>
class Unordered
{
public:
	Unordered(void)
	{}
	~Unordered(void)
	{
		clear();
	}

public:
	void insert(Key k, Value v)
	{
		WriteLock wl{ mtx };
		umap.insert(std::make_pair(k, v));
	}

	void remove(Key k)
	{
		WriteLock wl{ mtx };
		typename boost::unordered_map<Key, Value>::const_iterator it{ umap.find(k) };

		if (umap.end() != it)
		{
			umap.erase(it);
		}
	}

	void removeHas(const std::string name)
	{
		WriteLock wl{ mtx };
		for (typename boost::unordered_map<Key, Value>::iterator it = umap.begin(); it != umap.end();)
		{
			if (-1 == it->first.find(name))
			{
				it = umap.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void clear(void)
	{
		WriteLock wl{ mtx };
		umap.clear();
	}
	
	Value at(Key k)
	{
		ReadLock rl{ mtx };
		Value ret{};
		typename boost::unordered_map<Key, Value>::const_iterator it{ umap.find(k) };

		if (umap.end() != it)
		{
			ret = it->second;
		}

		return ret;
	}

	unsigned long long size(void)
	{
		ReadLock rl{ mtx };
		return umap.size();
	}

	void values(std::vector<Value>& data)
	{
		WriteLock wl{ mtx };
		for (typename boost::unordered_map<Key, Value>::const_iterator it = umap.cbegin(); it != umap.cend(); ++it)
		{
			data.push_back(it->second);
		}
	}

private:
	boost::unordered_map<Key, Value> umap;
	SharedMutex mtx;
};//class UnorderedMap

#endif//FRAMEWORK_LIBCOMMON_UNORDERED_MAP_H

