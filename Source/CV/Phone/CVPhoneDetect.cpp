#include "boost/bind.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/format.hpp"
#include "boost/pointer_cast.hpp"
#include "boost/thread.hpp"
#include "CV/PhoneClass/include/BasePhoneDll.h"
#include "Define.h"
#include "Error.h"
#include "Thread/ThreadPool.h"
using ThreadPool = framework::thread::ThreadPool;
#include "Data/Frame/Frame.h"
using Frame = framework::data::Frame;
using FramePtr = boost::shared_ptr<Frame>;
#include "CV/Phone/CVPhoneDetect.h"

CVPhoneDetect::CVPhoneDetect(
	CVResultNotificationCallback callback /* = nullptr */)
	: phone{ nullptr }, flag{ false }, cvresultNotificationCallback{ callback }
{}
CVPhoneDetect::~CVPhoneDetect() {}

int CVPhoneDetect::init(void* param /* = nullptr */)
{
	int e{ param ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		CVParam* cvp{ reinterpret_cast<CVParam*>(param) };
		const std::string filePath{
			boost::filesystem::initial_path<boost::filesystem::path>().string() };
		const std::string cfgFilePath{
			(boost::format("%s/model/phone.cfg") % filePath).str() };
		const std::string weightFilePath{
			(boost::format("%s/model/phone.weights") % filePath).str() };
// 		const std::string weightFilePath2{
// 			(boost::format("%s/model/phonesecond.weights") % filePath).str() };

		StruInitParams initParams1;
		initParams1.gpu_id = cvp->gpuID;
		initParams1.detectThreshold = cvp->detectThreshold;
		initParams1.trackThreshold = cvp->trackThreshold;
		initParams1.savePath = (char*)filePath.c_str();
		initParams1.cfgfile = (char*)cfgFilePath.c_str();
		initParams1.weightFile = (char*)weightFilePath.c_str();
		ownID.append(cvp->ownID);

// 		StruInitParams initParams2;
// 		initParams2.gpu_id = initParams1.gpu_id;
// 		initParams2.detectThreshold = 0.6f;//0.9f
// 		initParams2.cfgfile = initParams1.cfgfile;
// 		initParams2.weightFile = (char*)weightFilePath2.c_str();
// 		int w{ static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_WIDTH) },
// 			h{ static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_HEIGHT) };
// 
// 		int e1{
// 			cvPhoneAlgo.InitAlgoriParam(
// 				static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_WIDTH),
// 				static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_HEIGHT),
// 				static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_CHANNEL),
// 				initParams1) ? eSuccess : eBadInitSDK };
// 		int e2{
// 			cvPhoneGpuDetect.InitSetup(
// 				static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_WIDTH),
// 				static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_HEIGHT),
// 				static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_CHANNEL),
// 				initParams2,
// 				w,
// 				h) ? eSuccess : eBadInitSDK };

		CPhoneClass* pp{ new(std::nothrow) CPhoneClass };
		if (pp)
		{
			pp->InitAlgoriParam(1920, 1080, initParams1);
			phone = pp;
			ThreadPool::get_mutable_instance().createNewThread(
				boost::bind(&CVPhoneDetect::processingCalculateData, this));
		}

//		e = eSuccess == e1 && eSuccess == e2 ? AbstractAlgorithm::createNewAlgorithm() : eBadOperate;
	}

	return e;
}

int CVPhoneDetect::inputData(DataPtr data)
{
	int e{ data ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		if (phone && !flag)
		{
			//缓存连续的8帧数据
			fifo.pushBack(data);
			flag = (8 == fifo.size() ? true : false);
		}
		else
		{
			e = eBadOperate;
		}
	}

	return e;
}

void CVPhoneDetect::processingCalculateData()
{
	FeedBackPhone fb;
	CPhoneClass* pp{ reinterpret_cast<CPhoneClass*>(phone) };
// 	FILE* fd{ nullptr };
// 	fd = fopen("test.rgb24", "wb+");

	while (pp)
	{
		if (flag)
		{
			DataPtr data{ fifo.front() };

			if (data)
			{
				pp->MainProcFunc((unsigned char*)data->getData(), fb);
//				fwrite(data->getData(), data->getDataBytes(), 1, fd);

				for (std::map<int, StruMemoryInfo>::iterator iter = fb.mapMemory.begin(); iter != fb.mapMemory.end();)
				{
					//				if (iter->second.bDone)
					{
						// 计算最大的detectConfidence 和 fTrackConfidence;
						int nPhoneNum = 0;
						int nZuojiNum = 0;
						float maxConfidence = 0.f;
						int nSaveId = 0;
						for (int i = 0; i < iter->second.vecSaveMat.size(); i++)
						{
							// 增加第二次识别过滤--对第一次检测的结果进行扩张后，进行第二次确认
// 							cv::Mat extractMat(1080, 1920, CV_8UC3, iter->second.vecSaveMat[i].pUcharImage);
// 							cv::Rect expandRect;
// 							expandRect.x = (cv::max)(0, int(iter->second.vecSaveMat[i].rRect.x - iter->second.vecSaveMat[i].rRect.width * 0.1));
// 							expandRect.y = (cv::max)(0, int(iter->second.vecSaveMat[i].rRect.y - iter->second.vecSaveMat[i].rRect.height * 0.1));
// 							expandRect.width = (cv::min)(extractMat.cols - expandRect.x, int(iter->second.vecSaveMat[i].rRect.width * 1.2));
// 							expandRect.height = (cv::min)(extractMat.rows - expandRect.y, int(iter->second.vecSaveMat[i].rRect.height * 1.2));
// 
// 							if (0 > expandRect.x || 0 > expandRect.y || 0 > expandRect.width || 0 > expandRect.height)
// 							{
// 								continue;
// 							}
// 
// 							cv::Mat roiMat = extractMat(expandRect).clone();
// 							std::vector<StruDetectResult> secondResult;
// 							nowProcTime = GetTickCount64();
// 							gpuDectect.MultiObjectDetect(roiMat, secondResult);
// 							//						printf("=====  Phone.MultiObjectDetect run time = %lld.\r\n", GetTickCount64() - nowProcTime);
// 
// 							float widthScale2 = roiMat.cols * 1.0f / 416;
// 							float heightScale2 = roiMat.rows * 1.0f / 416;
// 							if (!secondResult.empty())
// 							{
								if (/*secondResult*/iter->second.vecSaveMat[0].nLabel == 0)
								{
									nPhoneNum++;
								}
								else if (/*secondResult*/iter->second.vecSaveMat[0].nLabel == 1)
								{
									nZuojiNum++;
								}
//							}
// 							else
// 								continue;

							if (iter->second.vecSaveMat[i].detectConfidence > maxConfidence/* && secondResult[0].nLabel == 0*/)
							{
								maxConfidence = iter->second.vecSaveMat[i].detectConfidence;
								nSaveId = i;
							}
						}

						if (nPhoneNum > 0 || nZuojiNum > 0)
						{
							FramePtr fp{
								boost::dynamic_pointer_cast<Frame>(data) };
							framework::data::OTP pos{
								iter->second.vecSaveMat[nSaveId].rRect.x,
								iter->second.vecSaveMat[nSaveId].rRect.y,
								iter->second.vecSaveMat[nSaveId].rRect.width,
								iter->second.vecSaveMat[nSaveId].rRect.height };
							fp->addOT(pos);
							fp->setFrameID(ownID);

							if (cvresultNotificationCallback)
							{
								cvresultNotificationCallback(data);
							}
						}

						for (int i = 0; i < iter->second.vecSaveMat.size(); i++)
						{
							if (nullptr != iter->second.vecSaveMat[i].pUcharImage)
								delete[] iter->second.vecSaveMat[i].pUcharImage;
						}
						iter = fb.mapMemory.erase(iter);
					}
				}
			}
			else
			{
				flag = false;
			}

			fifo.popFront();
		}
		else
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		}
	}
}
