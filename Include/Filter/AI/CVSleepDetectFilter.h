//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-05
//		Description :					˯���㷨Filter��
//
//		History:						Author									Date										Description
//										������									2020-10-05									����
//

#ifndef BASE_STREAM_CV_SLEEP_DETECT_FILTER_H
#define BASE_STREAM_CV_SLEEP_DETECT_FILTER_H

#include "Filter/AI/AIFilter.h"

namespace base
{
	namespace stream
	{
		class CVSleepDetectFilter : public AIFilter
		{
		public:
			CVSleepDetectFilter(void);
			virtual ~CVSleepDetectFilter(void);

		protected:
			int createNewFilter(void* param = nullptr) override;
			int destroyFilter(void) override;
		};//class CVSleepDetectFilter
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_CV_SLEEP_DETECT_FILTER_H
