//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					抽象Pin类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
//

#ifndef BASE_STREAM_ABSTRACT_PIN_H
#define BASE_STREAM_ABSTRACT_PIN_H

#include "boost/shared_ptr.hpp"
#include "Packet/Stream/StreamPacket.h"
using StreamPacket = base::packet::StreamPacket;
using StreamPacketPtr = boost::shared_ptr<StreamPacket>;

namespace base
{
	namespace stream
	{
		typedef enum class tagPinType_t : int
		{
			//Pin类型不存在None的情况
			PIN_TYPE_INPUT = 0,
			PIN_TYPE_OUTPUT
		}PinType;

		class AbstractFilter;
		using AbstractFilterPtr = boost::shared_ptr<AbstractFilter>;

		class AbstractPin
		{
			using AbstractPinPtr = boost::shared_ptr<AbstractPin>;

		public:
			AbstractPin(
				AbstractFilterPtr filter,
				const PinType type = PinType::PIN_TYPE_INPUT);
			virtual ~AbstractPin(void);

		public:
			//连接输出Pin到下一个输入Pin
			//@inputPin : 下一个输入Pin
			//@Return : 错误码
			//@Comment : 该方法只有当实例为输入类型时才能使用
			int connectPin(AbstractPinPtr inputPin);

			//输入流数据包
			//@pkt : 流数据包
			//@Return : 错误码
			int inputData(StreamPacketPtr pkt);

			//获取Pin类型
			//@Return : Pin类型
			inline const PinType getPinType(void) const
			{
				return pinType;
			}

		private:
			const PinType pinType;
			AbstractFilterPtr parentFilter;
			AbstractPinPtr nextInputPin;
		};//class AbstractPin
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_ABSTRACT_PIN_H
