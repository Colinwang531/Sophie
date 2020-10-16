#include "Error.h"
#include "Graph/AbstractGraph.h"

namespace base
{
	namespace stream
	{
		AbstractGraph::AbstractGraph()
		{}

		AbstractGraph::~AbstractGraph()
		{
			destroyGraph();
		}

		int AbstractGraph::createNewGraph(void* param /* = nullptr */)
		{
			int e{ createNewFilter(param) };

			if (eSuccess == e)
			{
				e = buildupGraph();
			}

			return e;
		}

		int AbstractGraph::destroyGraph()
		{
			abstractFilterGroup.clear();
			return eSuccess;
		}

		int AbstractGraph::addFilterByName(
			const std::string name, 
			AbstractFilterPtr filter)
		{
			int e{ name.empty() || !filter ? eInvalidParameter : eSuccess };

			if (eSuccess == e)
			{
				abstractFilterGroup.insert(name, filter);
			}

			return e;
		}

		AbstractFilterPtr AbstractGraph::getFilterByName(const std::string name)
		{
			return abstractFilterGroup.at(name);
		}
	}//namespace stream
}//namespace base
