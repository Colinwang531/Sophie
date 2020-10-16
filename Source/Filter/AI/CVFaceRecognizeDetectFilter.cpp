#include "boost/make_shared.hpp"
#include "Error.h"
#include "AI/CVFaceRecognizeDetect.h"
using CVFaceRecognizeDetect = base::ai::CVFaceRecognizeDetect;
using CVFaceRecognizeDetectPtr = boost::shared_ptr<CVFaceRecognizeDetect>;
#include "Filter/AI/CVFaceRecognizeDetectFilter.h"

namespace base
{
	namespace stream
	{
		CVFaceRecognizeDetectFilter::CVFaceRecognizeDetectFilter() : AIFilter()
		{}

		CVFaceRecognizeDetectFilter::~CVFaceRecognizeDetectFilter()
		{}

		int CVFaceRecognizeDetectFilter::addNewFaceImage(
			const int uid /* = -1 */,
			const char* image /* = nullptr */,
			const int bytes /* = 0 */)
		{
			int e{ -1 < uid && image && 0 < bytes ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				CVFaceRecognizeDetectPtr face{ 
					boost::dynamic_pointer_cast<CVFaceRecognizeDetect>(abstractAlgorithmPtr) };
				e = face->addNewFaceImage(uid, image, bytes);
			}

			return e;
		}

		int CVFaceRecognizeDetectFilter::createNewFilter(void* param /* = nullptr */)
		{
			int e{ AbstractFilter::createNewFilter(param) };

			if (eSuccess == e)
			{
				AbstractAlgorithm* aa{ reinterpret_cast<AbstractAlgorithm*>(param) };
				abstractAlgorithmPtr = boost::make_shared<CVFaceRecognizeDetect>(*this, aa->getAlgorithmType());
				if (abstractAlgorithmPtr)
				{
					e = abstractAlgorithmPtr->createNewAlgorithm(param);
				}
				else
				{
					e = eBadNewObject;
				}
			}

			return e;
		}

		int CVFaceRecognizeDetectFilter::destroyFilter()
		{
			int e{ abstractAlgorithmPtr ? eSuccess : eBadOperate };

			if (eSuccess == e)
			{
				e = abstractAlgorithmPtr->destroyAlgorithm();
			}

			return eSuccess == e ? AbstractFilter::destroyFilter() : e;
		}
	}//namespace stream
}//namespace base
