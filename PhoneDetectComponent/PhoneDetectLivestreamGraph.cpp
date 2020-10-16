#include "boost/make_shared.hpp"
#include "Filter/AI/CVPhoneDetectFilter.h"
using CVPhoneDetectFilter = base::stream::CVPhoneDetectFilter;
#include "PhoneDetectLivestreamGraph.h"

PhoneDetectLivestreamGraph::PhoneDetectLivestreamGraph()
	: LivestreamAIGraph()
{}

PhoneDetectLivestreamGraph::~PhoneDetectLivestreamGraph()
{}

AbstractFilterPtr PhoneDetectLivestreamGraph::createNewAIFilter()
{
	return boost::make_shared<CVPhoneDetectFilter>();
}
