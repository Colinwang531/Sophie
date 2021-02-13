//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					多媒体图抽象类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
//

#ifndef FRAMEWORK_MULTIMEDIA_GRAPH_H
#define FRAMEWORK_MULTIMEDIA_GRAPH_H

#include "DataStruct/UnorderedMap.h"

namespace framework
{
	namespace multimedia
	{
		class Filter;
		using FilterPtr = boost::shared_ptr<Filter>;
		using FilterPtrGroup = UnorderedMap<std::string, FilterPtr>;

		class Graph
		{
		public:
			Graph(void);
			virtual ~Graph(void);

		public:
			//查询过滤器
			//@name : 名称
			//@Return : 过滤器实例
			FilterPtr queryFilter(const std::string name);

			//运行图
			//@Return : 错误码
			virtual int runGraph(void) = 0;

		protected:
			//添加过滤器
			//@name : 名称
			//@Return : 错误码
			virtual int addFilter(const std::string name) = 0;

			//删除过滤器
			//@name : 名称
			//@Return : 错误码
			virtual int removeFilter(const std::string name) = 0;

		protected:
			FilterPtrGroup filters;
		};//class Graph
	}//namespace multimedia
}//namespace framework

#endif//FRAMEWORK_MULTIMEDIA_GRAPH_H
