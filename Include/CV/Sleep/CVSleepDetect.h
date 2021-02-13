//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-05
//		Description :					睡觉检测类
//
//		History:						Author									Date										Description
//										王科威									2020-10-05									创建
//

#ifndef ARTIFICIAL_INTELLIGENCE_COMPUTOR_VISION_SLEEP_H
#define ARTIFICIAL_INTELLIGENCE_COMPUTOR_VISION_SLEEP_H

#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include "DataStruct/FIFOQueue.h"
#include "Data/Data.h"
using Data = framework::data::Data;
using DataPtr = boost::shared_ptr<Data>;

typedef boost::function<void(DataPtr)> CVResultNotificationCallback;

class CVSleepDetect
{
public:
	CVSleepDetect(
		CVResultNotificationCallback callback = nullptr);
	virtual ~CVSleepDetect(void);

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
	void* sleep;
	bool flag;
	FIFOQueue<DataPtr> fifo;
	CVResultNotificationCallback cvresultNotificationCallback;
	unsigned long long lastObjectDetectTickcount;
};//class CVSleepDetect

#endif//ARTIFICIAL_INTELLIGENCE_COMPUTOR_VISION_SLEEP_H
