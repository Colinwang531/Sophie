#include "boost/make_shared.hpp"
#include "Filter/AI/CVSleepDetectFilter.h"
using CVSleepDetectFilter = base::stream::CVSleepDetectFilter;
#include "SleepDetectLivestreamGraph.h"

SleepDetectLivestreamGraph::SleepDetectLivestreamGraph()
	: LivestreamAIGraph()
{}

SleepDetectLivestreamGraph::~SleepDetectLivestreamGraph()
{}

AbstractFilterPtr SleepDetectLivestreamGraph::createNewAIFilter()
{
	return boost::make_shared<CVSleepDetectFilter>();
}
