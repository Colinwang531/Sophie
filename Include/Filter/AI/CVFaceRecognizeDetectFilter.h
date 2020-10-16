//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-05
//		Description :					人脸识别算法Filter类
//
//		History:						Author									Date										Description
//										王科威									2020-10-05									创建
//

#ifndef BASE_STREAM_CV_FACE_RECOGNIZE_DETECT_FILTER_H
#define BASE_STREAM_CV_FACE_RECOGNIZE_DETECT_FILTER_H

#include "Filter/AI/AIFilter.h"

namespace base
{
	namespace stream
	{
		class CVFaceRecognizeDetectFilter : public AIFilter
		{
		public:
			CVFaceRecognizeDetectFilter(void);
			virtual ~CVFaceRecognizeDetectFilter(void);

		public:
			//添加人脸图片
			//@uid : 人脸图片ID标识
			//@image : 人脸图片数据
			//@bytes : 人脸图片大小
			//@Return : 错误码
			//@Comment : 由于现有算法接口的局限,新添加的图片必须先要保存到
			//			 本地目录下再通过算法接口读取文件的方式进行添加
			int addNewFaceImage(
				const int uid = -1,
				const char* image = nullptr,
				const int bytes = 0);

		protected:
			int createNewFilter(void* param = nullptr) override;
			int destroyFilter(void) override;
		};//class CVFaceRecognizeDetectFilter
	}//namespace stream
}//namespace base

#endif//BASE_STREAM_CV_FACE_RECOGNIZE_DETECT_FILTER_H
