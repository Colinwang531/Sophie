//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					抽象Filter类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
//

#ifndef BASE_STREAM_ABSTRACT_FILTER_H
#define BASE_STREAM_ABSTRACT_FILTER_H

#include "boost/enable_shared_from_this.hpp"
#include "boost/function.hpp"
#include "DataStruct/UnorderedMap.h"
#include "Packet/Stream/StreamPacket.h"
using StreamPacket = base::packet::StreamPacket;
using StreamPacketPtr = boost::shared_ptr<StreamPacket>;

namespace base
{
	namespace stream
	{
		typedef enum class tagFilterType_t : int
		{
			FILTER_TYPE_MEDIUM = 0,
			FILTER_TYPE_SOURCE, 
			FILTER_TYPE_TARGET
		}FilterType;

		class AbstractPin;
		using AbstractPinPtr = boost::shared_ptr<AbstractPin>;
		using AbstractPinPtrGroup = UnorderedMap<const std::string, AbstractPinPtr>;
		typedef boost::function<void(StreamPacketPtr)> StreamPacketNotificationCallback;

		class AbstractFilter
			: public boost::enable_shared_from_this<AbstractFilter>
		{
		public:
			AbstractFilter(
				const FilterType type = FilterType::FILTER_TYPE_MEDIUM);
			virtual ~AbstractFilter(void);

		public:
			//创建和销毁Filter实例内部所需要的其他东西
			//@param : 用户参数
			//@Return : 错误码
			virtual int createNewFilter(void* param = nullptr);
			virtual int destroyFilter(void);

			//设置Graph实例数据输出回调
			inline void setDataCallback(StreamPacketNotificationCallback callback = nullptr)
			{
				streamDataCallback = callback;
			}

			//输入流数据包
			//@pkt : 流数据包
			//@Return : 错误码
			//@Comment : 当前类的实例在调用该方法时无法确定输出Pin实例是那些
			//			 且不知道该向那些Pin数据输入数据,所以把该方法的实现交给
			//			 继承者自己完成
			virtual int inputData(StreamPacketPtr pkt) = 0;

			//获取Filter类型
			//@Return : Filter类型
			inline const FilterType getFilterType(void) const
			{
				return filterType;
			}

			//获取Pin实例
			//@name : 名称
			//@Return : Pin实例
			AbstractPinPtr getPinByName(const std::string name);

			//处理输入数据后的通知
			//pkt[out] : 处理输入数据后产生的新数据
			//           新数据类型由不同的Filter实例创建
			//@Return : 错误码
			virtual int afterProcessInputDataNotification(StreamPacketPtr pkt) = 0;

		protected:
			//创建Pin实例
			//@Return : 错误码
			virtual int createNewPin(void) = 0;

		protected:
			StreamPacketNotificationCallback streamDataCallback;
			AbstractPinPtrGroup abstractPinGroup;
			const FilterType filterType;
		};//class AbstractFilter
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_ABSTRACT_FILTER_H
