//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-01
//		Description :					����������
//
//		History:						Author									Date										Description
//										������									2020-07-01									����
//

#ifndef BASE_COMPONENT_ABSTRACT_COMPONENT_H
#define BASE_COMPONENT_ABSTRACT_COMPONENT_H

#include "boost/noncopyable.hpp"

namespace base
{
	namespace component
	{
		typedef enum class tagComponentType_t : int
		{
			COMPONENT_TYPE_NONE = 0,
			COMPONENT_TYPE_XMQ = 1,
			COMPONENT_TYPE_WEB = 2,
			COMPONENT_TYPE_HKD = 3,
			COMPONENT_TYPE_DHD = 4,
			COMPONENT_TYPE_ALM = 5,
			COMPONENT_TYPE_AI = 6
		}ComponentType;

		class AbstractComponent : private boost::noncopyable
		{
		public:
			AbstractComponent(
				const ComponentType type = ComponentType::COMPONENT_TYPE_NONE);
			virtual ~AbstractComponent(void);

		public:
			//��ȡ�������
			//@Return : �������
			inline const ComponentType getComponentType(void) const
			{
				return componentType;
			}

			//��/д���ID
			//@name : ���ID,���Կձ�ʾע������
			//@Return : ���ID
			inline void setComponentID(const std::string id)
			{
				componentID = id;
			}
			inline const std::string getComponentID(void) const
			{
				return componentID;
			}

			//��/дͨ��ID
			//@name : ͨ��ID,���Կ�
			//@Return : ͨ��ID
			inline void setCommunicationID(const std::string id)
			{
				communicationID = id;
			}
			inline const std::string getCommunicationID(void) const
			{
				return communicationID;
			}

			//��/д�������
			//@name : �������,���Կ�
			//@Return : �������
			inline void setComponentName(const std::string name)
			{
				componentName = name;
			}
			inline const std::string getComponentName(void) const
			{
				return componentName;
			}

			//��/д���ʱ���
			//@ts : ʱ���
			//@Return : ���ʱ���
			inline void setComponentTimestamp(const long long ts)
			{
				keepliveTimestamp = ts;
			}
			inline const long long getComponentTimestamp(void) const
			{
				return keepliveTimestamp;
			}

		private:
			const ComponentType componentType;
			std::string componentID;
			//ͨ��ID��ʶ���������������������
			std::string communicationID;
			std::string componentName;
			long long keepliveTimestamp;
		};//class AbstractComponent
	}//namespace component
}//namespace base

#endif//BASE_COMPONENT_ABSTRACT_COMPONENT_H
