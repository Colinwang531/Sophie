//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					安全帽算法Filter类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
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
