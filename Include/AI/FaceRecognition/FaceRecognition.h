//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-20
//		Description :					人脸识别类
//
//		History:						Author									Date										Description
//										王科威									2020-07-20									创建
//

#ifndef BASE_AI_FACE_RECOGNITION_H
#define BASE_AI_FACE_RECOGNITION_H

#include "AI/AbstractAlgorithm.h"

namespace base
{
	namespace ai
	{
		class FaceRecognition : public AbstractAlgorithm
		{
		public:
			FaceRecognition(
				const std::string cid,
				const AlgorithmType type = AlgorithmType::ALGORITHM_TYPE_NONE,
				const int gpu = 0);
			virtual ~FaceRecognition(void);

		protected:
			//读/写人脸检测相似度阈值
			//@similar : 相似度阈值
			//@Return : 错误码
			//@Comment : 该方法仅人脸算法实现
			int setFaceCompareSimilar(const float similar = 0.52f) override;
			int getFaceCompareSimilar(float& similar) const override;

		private:
			float similarThreshold;
		};//class FaceRecognition
	}//namespace ai
}//namespace base

#endif//BASE_AI_FACE_RECOGNITION_H