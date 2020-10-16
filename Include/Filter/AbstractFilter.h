//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					����Filter��
//
//		History:						Author									Date										Description
//										������									2020-09-23									����
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
			//����������Filterʵ���ڲ�����Ҫ����������
			//@param : �û�����
			//@Return : ������
			virtual int createNewFilter(void* param = nullptr);
			virtual int destroyFilter(void);

			//����Graphʵ����������ص�
			inline void setDataCallback(StreamPacketNotificationCallback callback = nullptr)
			{
				streamDataCallback = callback;
			}

			//���������ݰ�
			//@pkt : �����ݰ�
			//@Return : ������
			//@Comment : ��ǰ���ʵ���ڵ��ø÷���ʱ�޷�ȷ�����Pinʵ������Щ
			//			 �Ҳ�֪��������ЩPin������������,���԰Ѹ÷�����ʵ�ֽ���
			//			 �̳����Լ����
			virtual int inputData(StreamPacketPtr pkt) = 0;

			//��ȡFilter����
			//@Return : Filter����
			inline const FilterType getFilterType(void) const
			{
				return filterType;
			}

			//��ȡPinʵ��
			//@name : ����
			//@Return : Pinʵ��
			AbstractPinPtr getPinByName(const std::string name);

			//�����������ݺ��֪ͨ
			//pkt[out] : �����������ݺ������������
			//           �����������ɲ�ͬ��Filterʵ������
			//@Return : ������
			virtual int afterProcessInputDataNotification(StreamPacketPtr pkt) = 0;

		protected:
			//����Pinʵ��
			//@Return : ������
			virtual int createNewPin(void) = 0;

		protected:
			StreamPacketNotificationCallback streamDataCallback;
			AbstractPinPtrGroup abstractPinGroup;
			const FilterType filterType;
		};//class AbstractFilter
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_ABSTRACT_FILTER_H
