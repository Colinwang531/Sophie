#include "Time/XTime.h"
using base::time::Time;
#include "Component/AbstractComponent.h"

namespace base
{
	namespace component
	{
		AbstractComponent::AbstractComponent(
			const ComponentType type /* = ComponentType::COMPONENT_TYPE_NONE */)
			: componentType{ type }, keepliveTimestamp{ Time().tickcount() }
		{}

		AbstractComponent::~AbstractComponent(){}
	}//namespace component
}//namespace base
