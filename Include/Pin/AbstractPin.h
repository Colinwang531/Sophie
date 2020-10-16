//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					����Pin��
//
//		History:						Author									Date										Description
//										������									2020-09-23									����
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
			//Pin���Ͳ�����None�����
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
			//�������Pin����һ������Pin
			//@inputPin : ��һ������Pin
			//@Return : ������
			//@Comment : �÷���ֻ�е�ʵ��Ϊ��������ʱ����ʹ��
			int connectPin(AbstractPinPtr inputPin);

			//���������ݰ�
			//@pkt : �����ݰ�
			//@Return : ������
			int inputData(StreamPacketPtr pkt);

			//��ȡPin����
			//@Return : Pin����
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
