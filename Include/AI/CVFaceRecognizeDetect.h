//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-05
//		Description :					����ʶ������
//
//		History:						Author									Date										Description
//										������									2020-10-05									����
//

#ifndef BASE_AI_CV_FACE_RECOGNIZE_DETECT_H
#define BASE_AI_CV_FACE_RECOGNIZE_DETECT_H

#include "AI/BaseFaceDll.h"
#include "DataStruct/FIFOQueue.h"
#include "Filter/AbstractFilter.h"
using AbstractFilter = base::stream::AbstractFilter;
#include "AI/AbstractAlgorithm.h"

namespace base
{
	namespace ai
	{
		class CVFaceRecognizeDetect : public AbstractAlgorithm
		{
		public:
			CVFaceRecognizeDetect(
				AbstractFilter& filter,
				const AlgorithmType type = AlgorithmType::ALGORITHM_TYPE_ATTENDANCE_IN);
			virtual ~CVFaceRecognizeDetect(void);

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
			int createNewAlgorithm(void* param = nullptr) override;
			int destroyAlgorithm(void) override;
			int inputData(StreamPacketPtr pkt) override;
			void processingCalculateData(void) override;

		private:
			AbstractFilter& abstractFilter;
			CFaceAlgorithmClass cvFaceRecognizeAlgo;
			FeedBackFaceRecog feedbackFaceRecognize;
			FIFOQueue<StreamPacketPtr> imageDataGroup;
			bool enableDetect;
			CRITICAL_SECTION cs;
		};//class CVFaceRecognizeDetect
	}//namespace ai
}//namespace base

#endif//BASE_AI_CV_FACE_RECOGNIZE_DETECT_H
