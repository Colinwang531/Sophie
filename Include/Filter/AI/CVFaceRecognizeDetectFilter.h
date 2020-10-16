//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-05
//		Description :					����ʶ���㷨Filter��
//
//		History:						Author									Date										Description
//										������									2020-10-05									����
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
			//�������ͼƬ
			//@uid : ����ͼƬID��ʶ
			//@image : ����ͼƬ����
			//@bytes : ����ͼƬ��С
			//@Return : ������
			//@Comment : ���������㷨�ӿڵľ���,����ӵ�ͼƬ������Ҫ���浽
			//			 ����Ŀ¼����ͨ���㷨�ӿڶ�ȡ�ļ��ķ�ʽ�������
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
