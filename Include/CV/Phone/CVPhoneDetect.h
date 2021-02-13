//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-05
//		Description :					��绰�����
//
//		History:						Author									Date										Description
//										������									2020-10-05									����
//

#ifndef ARTIFICIAL_INTELLIGENCE_COMPUTOR_VISION_PHONE_H
#define ARTIFICIAL_INTELLIGENCE_COMPUTOR_VISION_PHONE_H

#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include "DataStruct/FIFOQueue.h"
#include "Data/Data.h"
using Data = framework::data::Data;
using DataPtr = boost::shared_ptr<Data>;

typedef boost::function<void(DataPtr)> CVResultNotificationCallback;

class CVPhoneDetect
{
public:
	CVPhoneDetect(
		CVResultNotificationCallback callback = nullptr);
	~CVPhoneDetect(void);

public:
	//��ʼ��
	//@param : ����
	//@Return : ������
	int init(void* param = nullptr);

	//��������
	//@data : ����
	//@Return : ������
	int inputData(DataPtr data);

protected:
	//�㷨�����߳�
	void processingCalculateData(void);

private:
	std::string ownID;
	void* phone;
	bool flag;
	FIFOQueue<DataPtr> fifo;
	CVResultNotificationCallback cvresultNotificationCallback;
};//class CVPhoneDetect
#endif//ARTIFICIAL_INTELLIGENCE_COMPUTOR_VISION_PHONE_H
