#include "Error.h"
#include "Graph/Graph.h"

namespace framework
{
	namespace multimedia
	{
		Graph::Graph()
		{}
		Graph::~Graph()
		{
			filters.clear();
		}

		FilterPtr Graph::queryFilter(const std::string name)
		{
			return filters.at(name);
		}
	}//namespace multimedia
}//namespace framework
