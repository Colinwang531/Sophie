//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					��ý��ͼ������
//
//		History:						Author									Date										Description
//										������									2020-09-23									����
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
			//��ѯ������
			//@name : ����
			//@Return : ������ʵ��
			FilterPtr queryFilter(const std::string name);

			//����ͼ
			//@Return : ������
			virtual int runGraph(void) = 0;

		protected:
			//��ӹ�����
			//@name : ����
			//@Return : ������
			virtual int addFilter(const std::string name) = 0;

			//ɾ��������
			//@name : ����
			//@Return : ������
			virtual int removeFilter(const std::string name) = 0;

		protected:
			FilterPtrGroup filters;
		};//class Graph
	}//namespace multimedia
}//namespace framework

#endif//FRAMEWORK_MULTIMEDIA_GRAPH_H
