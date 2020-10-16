#include "boost/make_shared.hpp"
#include "Error.h"
#include "AI/CVHelmetDetect.h"
using CVHelmetDetect = base::ai::CVHelmetDetect;
#include "Filter/AI/CVHelmetDetectFilter.h"

namespace base
{
	namespace stream
	{
		CVHelmetDetectFilter::CVHelmetDetectFilter() : AIFilter()
		{}

		CVHelmetDetectFilter::~CVHelmetDetectFilter()
		{}

		int CVHelmetDetectFilter::createNewFilter(void* param /* = nullptr */)
		{
			int e{ AbstractFilter::createNewFilter(param) };

			if (eSuccess == e)
			{
				abstractAlgorithmPtr = boost::make_shared<CVHelmetDetect>(*this);
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

		int CVHelmetDetectFilter::destroyFilter()
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
