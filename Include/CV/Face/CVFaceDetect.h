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
	//初始化
	//@param : 参数
	//@Return : 错误码
	int init(void* param = nullptr);

	//输入数据
	//@data : 数据
	//@Return : 错误码
	int inputData(DataPtr data);

	//添加人脸底库图片
	//@param : 参数
	//@Return : 错误码
	int setFaceImage(void* param = nullptr);

protected:
	//算法计算线程
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
