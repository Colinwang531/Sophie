#include "boost/make_shared.hpp"
#include "Error.h"
#include "AI/CVSleepDetect.h"
using CVSleepDetect = base::ai::CVSleepDetect;
#include "Filter/AI/CVSleepDetectFilter.h"

namespace base
{
	namespace stream
	{
		CVSleepDetectFilter::CVSleepDetectFilter() : AIFilter()
		{}

		CVSleepDetectFilter::~CVSleepDetectFilter()
		{}

		int CVSleepDetectFilter::createNewFilter(void* param /* = nullptr */)
		{
			int e{ AbstractFilter::createNewFilter(param) };

			if (eSuccess == e)
			{
				abstractAlgorithmPtr = boost::make_shared<CVSleepDetect>(*this);
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

		int CVSleepDetectFilter::destroyFilter()
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
