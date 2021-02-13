//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-05
//		Description :					打电话检测类
//
//		History:						Author									Date										Description
//										王科威									2020-10-05									创建
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
	//初始化
	//@param : 参数
	//@Return : 错误码
	int init(void* param = nullptr);

	//输入数据
	//@data : 数据
	//@Return : 错误码
	int inputData(DataPtr data);

protected:
	//算法计算线程
	void processingCalculateData(void);

private:
	std::string ownID;
	void* phone;
	bool flag;
	FIFOQueue<DataPtr> fifo;
	CVResultNotificationCallback cvresultNotificationCallback;
};//class CVPhoneDetect
#endif//ARTIFICIAL_INTELLIGENCE_COMPUTOR_VISION_PHONE_H
