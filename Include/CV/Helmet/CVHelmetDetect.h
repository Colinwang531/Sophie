//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-18
//		Description :					安全帽检测类
//
//		History:						Author									Date										Description
//										王科威									2020-07-18									创建
//

#ifndef ARTIFICIAL_INTELLIGENCE_COMPUTOR_VISION_HELMET_H
#define ARTIFICIAL_INTELLIGENCE_COMPUTOR_VISION_HELMET_H

#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include "DataStruct/FIFOQueue.h"
#include "Data/Data.h"
using Data = framework::data::Data;
using DataPtr = boost::shared_ptr<Data>;

typedef boost::function<void(DataPtr)> CVResultNotificationCallback;

class CVHelmetDetect
{
public:
	CVHelmetDetect(
		CVResultNotificationCallback callback = nullptr);
	~CVHelmetDetect(void);

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
	void* helmet;
	bool flag;
	FIFOQueue<DataPtr> fifo;
	CVResultNotificationCallback cvresultNotificationCallback;
};//class CVHelmetDetect

#endif//ARTIFICIAL_INTELLIGENCE_COMPUTOR_VISION_HELMET_H
