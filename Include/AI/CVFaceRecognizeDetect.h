//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-05
//		Description :					人脸识别检测类
//
//		History:						Author									Date										Description
//										王科威									2020-10-05									创建
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
