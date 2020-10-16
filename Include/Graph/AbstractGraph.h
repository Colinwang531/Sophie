//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					抽象Graph类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
//

#ifndef BASE_STREAM_ABSTRACT_GRAPH_H
#define BASE_STREAM_ABSTRACT_GRAPH_H

#include "DataStruct/UnorderedMap.h"
#include "Packet/Stream/StreamPacket.h"
using StreamPacket = base::packet::StreamPacket;
using StreamPacketPtr = boost::shared_ptr<StreamPacket>;

namespace base
{
	namespace stream
	{
		class AbstractFilter;
		using AbstractFilterPtr = boost::shared_ptr<AbstractFilter>;
		using AbstractFilterPtrGroup = UnorderedMap<const std::string, AbstractFilterPtr>;

		class AbstractGraph
		{
		public:
			AbstractGraph(void);
			virtual ~AbstractGraph(void);

		public:
			//创建和销毁Graph实例内部所需要的其他东西
			//@param : 参数
			//@Return : 错误码
			int createNewGraph(void* param = nullptr);
			int destroyGraph(void);

			//输入流数据包
			//@pkt : 流数据包
			//@Return : 错误码
			virtual int inputData(StreamPacketPtr pkt) = 0;

			//添加Filter实例
			//@name : 名称
			//@filter : Filter实例
			//@Return : 错误码
			int addFilterByName(
				const std::string name,
				AbstractFilterPtr filter);

			//获取Filter实例
			//@name : 名称
			//@Return : Filter实例
			AbstractFilterPtr getFilterByName(const std::string name);

		protected:
			//创建Filter实例
			//@param : 参数
			//@Return : 错误码
			virtual int createNewFilter(void* param = nullptr) = 0;

			//构建Graph流程
			//@Return : 错误码
			virtual int buildupGraph(void) = 0;

		protected:
			AbstractFilterPtrGroup abstractFilterGroup;
		};//class AbstractGraph
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_ABSTRACT_GRAPH_H
