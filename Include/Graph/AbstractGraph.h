//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					����Graph��
//
//		History:						Author									Date										Description
//										������									2020-09-23									����
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
			//����������Graphʵ���ڲ�����Ҫ����������
			//@param : ����
			//@Return : ������
			int createNewGraph(void* param = nullptr);
			int destroyGraph(void);

			//���������ݰ�
			//@pkt : �����ݰ�
			//@Return : ������
			virtual int inputData(StreamPacketPtr pkt) = 0;

			//���Filterʵ��
			//@name : ����
			//@filter : Filterʵ��
			//@Return : ������
			int addFilterByName(
				const std::string name,
				AbstractFilterPtr filter);

			//��ȡFilterʵ��
			//@name : ����
			//@Return : Filterʵ��
			AbstractFilterPtr getFilterByName(const std::string name);

		protected:
			//����Filterʵ��
			//@param : ����
			//@Return : ������
			virtual int createNewFilter(void* param = nullptr) = 0;

			//����Graph����
			//@Return : ������
			virtual int buildupGraph(void) = 0;

		protected:
			AbstractFilterPtrGroup abstractFilterGroup;
		};//class AbstractGraph
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_ABSTRACT_GRAPH_H
