#include "boost/make_shared.hpp"
#include "Filter/AI/CVHelmetDetectFilter.h"
using CVHelmetDetectFilter = base::stream::CVHelmetDetectFilter;
#include "HelmetDetectLivestreamGraph.h"

HelmetDetectLivestreamGraph::HelmetDetectLivestreamGraph()
	: LivestreamAIGraph()
{}

HelmetDetectLivestreamGraph::~HelmetDetectLivestreamGraph()
{}

AbstractFilterPtr HelmetDetectLivestreamGraph::createNewAIFilter()
{
	return boost::make_shared<CVHelmetDetectFilter>();
}
