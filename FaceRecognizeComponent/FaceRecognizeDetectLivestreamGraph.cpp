#include "boost/make_shared.hpp"
#include "Filter/AI/CVFaceRecognizeDetectFilter.h"
using CVFaceRecognizeDetectFilter = base::stream::CVFaceRecognizeDetectFilter;
#include "FaceRecognizeDetectLivestreamGraph.h"

FaceRecognizeDetectLivestreamGraph::FaceRecognizeDetectLivestreamGraph()
	: LivestreamAIGraph()
{}

FaceRecognizeDetectLivestreamGraph::~FaceRecognizeDetectLivestreamGraph()
{}

AbstractFilterPtr FaceRecognizeDetectLivestreamGraph::createNewAIFilter()
{
	return boost::make_shared<CVFaceRecognizeDetectFilter>();
}
