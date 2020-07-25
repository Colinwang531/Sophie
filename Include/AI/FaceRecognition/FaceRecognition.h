//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-20
//		Description :					����ʶ����
//
//		History:						Author									Date										Description
//										������									2020-07-20									����
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
			//��/д����������ƶ���ֵ
			//@similar : ���ƶ���ֵ
			//@Return : ������
			//@Comment : �÷����������㷨ʵ��
			int setFaceCompareSimilar(const float similar = 0.52f) override;
			int getFaceCompareSimilar(float& similar) const override;

		private:
			float similarThreshold;
		};//class FaceRecognition
	}//namespace ai
}//namespace base

#endif//BASE_AI_FACE_RECOGNITION_H