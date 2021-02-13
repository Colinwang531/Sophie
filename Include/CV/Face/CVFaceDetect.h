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

#ifndef ARTIFICIAL_INTELLIGENCE_COMPUTOR_VISION_FACE_H
#define ARTIFICIAL_INTELLIGENCE_COMPUTOR_VISION_FACE_H

#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include "DataStruct/FIFOQueue.h"
#include "DataStruct/UnorderedMap.h"
#include "Data/Data.h"
using Data = framework::data::Data;
using DataPtr = boost::shared_ptr<Data>;

typedef boost::function<void(DataPtr)> CVResultNotificationCallback;

class CVFaceDetect
{
public:
	CVFaceDetect(
		CVResultNotificationCallback callback = nullptr);
	~CVFaceDetect(void);

public:
	//��ʼ��
	//@param : ����
	//@Return : ������
	int init(void* param = nullptr);

	//��������
	//@data : ����
	//@Return : ������
	int inputData(DataPtr data);

	//��������׿�ͼƬ
	//@param : ����
	//@Return : ������
	int setFaceImage(void* param = nullptr);

protected:
	//�㷨�����߳�
	void processingCalculateData(void);

private:
	std::string ownID;
	void* face;
	bool flag;
	FIFOQueue<DataPtr> fifo;
	UnorderedMap<int, std::string> faces;
	CVResultNotificationCallback cvresultNotificationCallback;
	int faceRegisterNumber;
};//class CVFaceDetect

#endif//ARTIFICIAL_INTELLIGENCE_COMPUTOR_VISION_FACE_H
