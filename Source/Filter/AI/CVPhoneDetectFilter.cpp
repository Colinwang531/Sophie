#include "boost/make_shared.hpp"
#include "Error.h"
#include "AI/CVPhoneDetect.h"
using CVPhoneDetect = base::ai::CVPhoneDetect;
#include "Filter/AI/CVPhoneDetectFilter.h"

namespace base
{
	namespace stream
	{
		CVPhoneDetectFilter::CVPhoneDetectFilter() : AIFilter()
		{}

		CVPhoneDetectFilter::~CVPhoneDetectFilter()
		{}

		int CVPhoneDetectFilter::createNewFilter(void* param /* = nullptr */)
		{
			int e{ AbstractFilter::createNewFilter(param) };

			if (eSuccess == e)
			{
				abstractAlgorithmPtr = boost::make_shared<CVPhoneDetect>(*this);
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

		int CVPhoneDetectFilter::destroyFilter()
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
