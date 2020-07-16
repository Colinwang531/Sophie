//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-01
//		Description :					组件抽象基类
//
//		History:						Author									Date										Description
//										王科威									2020-07-01									创建
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
			//获取组件类型
			//@Return : 组件类型
			inline const ComponentType getComponentType(void) const
			{
				return componentType;
			}

			//读/写组件ID
			//@name : 组件ID,可以空表示注册请求
			//@Return : 组件ID
			inline void setComponentID(const std::string id)
			{
				componentID = id;
			}
			inline const std::string getComponentID(void) const
			{
				return componentID;
			}

			//读/写通信ID
			//@name : 通信ID,可以空
			//@Return : 通信ID
			inline void setCommunicationID(const std::string id)
			{
				communicationID = id;
			}
			inline const std::string getCommunicationID(void) const
			{
				return communicationID;
			}

			//读/写组件名称
			//@name : 组件名称,可以空
			//@Return : 组件名称
			inline void setComponentName(const std::string name)
			{
				componentName = name;
			}
			inline const std::string getComponentName(void) const
			{
				return componentName;
			}

			//读/写组件时间戳
			//@ts : 时间戳
			//@Return : 组件时间戳
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
			//通信ID标识用于主动向组件发送数据
			std::string communicationID;
			std::string componentName;
			long long keepliveTimestamp;
		};//class AbstractComponent
	}//namespace component
}//namespace base

#endif//BASE_COMPONENT_ABSTRACT_COMPONENT_H
