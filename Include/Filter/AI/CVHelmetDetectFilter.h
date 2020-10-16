//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					��ȫñ�㷨Filter��
//
//		History:						Author									Date										Description
//										������									2020-09-23									����
//

#ifndef BASE_STREAM_CV_HELMET_DETECT_FILTER_H
#define BASE_STREAM_CV_HELMET_DETECT_FILTER_H

#include "Filter/AI/AIFilter.h"

namespace base
{
	namespace stream
	{
		class CVHelmetDetectFilter : public AIFilter
		{
		public:
			CVHelmetDetectFilter(void);
			virtual ~CVHelmetDetectFilter(void);

		protected:
			int createNewFilter(void* param = nullptr) override;
			int destroyFilter(void) override;
		};//class CVHelmetDetectFilter
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_CV_HELMET_DETECT_FILTER_H
