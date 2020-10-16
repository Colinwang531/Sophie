#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/format.hpp"
#include "boost/thread.hpp"
#include "opencv2/opencv.hpp"
#include "Error.h"
#include "AI/CVPhoneDetect.h"

namespace base
{
	namespace ai
	{
		CVPhoneDetect::CVPhoneDetect(AbstractFilter& filter)
			: AbstractAlgorithm(AlgorithmType::ALGORITHM_TYPE_PHONE), abstractFilter{ filter }, enableDetect{ false }
		{}
		CVPhoneDetect::~CVPhoneDetect(){}

		int CVPhoneDetect::createNewAlgorithm(void* param /* = nullptr */)
		{
			AbstractAlgorithm* aa{ reinterpret_cast<AbstractAlgorithm*>(param) };
			int e{ aa ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				const std::string filePath{
					boost::filesystem::initial_path<boost::filesystem::path>().string() };
				const std::string cfgFilePath{ 
					(boost::format("%s\\model\\phone.cfg") % filePath).str() };
				const std::string weightFilePath{ 
					(boost::format("%s\\model\\phone.weights") % filePath).str() };
				const std::string weightFilePath2{ 
					(boost::format("%s\\model\\phonesecond.weights") % filePath).str() };

				StruInitParams initParams1;
				initParams1.gpu_id = aa->getGpuID() - 1;
				initParams1.detectThreshold = aa->getFirstDetectThreshold();
				initParams1.trackThreshold = aa->getTrackThreshold();
				initParams1.savePath = (char*)filePath.c_str();
				initParams1.cfgfile = (char*)cfgFilePath.c_str();
				initParams1.weightFile = (char*)weightFilePath.c_str();
				StruInitParams initParams2;
				initParams2.gpu_id = initParams1.gpu_id;
				initParams2.detectThreshold = 0.6f;//0.9f
				initParams2.cfgfile = initParams1.cfgfile;
				initParams2.weightFile = (char*)weightFilePath2.c_str();
				int w{ static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_WIDTH) }, 
					h{ static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_HEIGHT) };

				int e1{
					cvPhoneAlgo.InitAlgoriParam(
						static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_WIDTH),
						static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_HEIGHT),
						static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_CHANNEL),
						initParams1) ? eSuccess : eBadInitSDK };
				int e2{
					cvPhoneGpuDetect.InitSetup(
						static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_WIDTH),
						static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_HEIGHT),
						static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_CHANNEL),
						initParams2,
						w, 
						h) ? eSuccess : eBadInitSDK };

				e = eSuccess == e1 && eSuccess == e2 ? AbstractAlgorithm::createNewAlgorithm() : eBadOperate;
			}

			return e;
		}

		int CVPhoneDetect::destroyAlgorithm()
		{
			return AbstractAlgorithm::destroyAlgorithm();
		}

		int CVPhoneDetect::inputData(StreamPacketPtr pkt)
		{
			int e{ pkt ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				if (!enableDetect)
				{
					//缓存连续的8帧数据
					imageDataGroup.pushBack(pkt);
					enableDetect = (8 == imageDataGroup.size() ? true : false);
				}
			}

			return e;
		}

		void CVPhoneDetect::processingCalculateData()
		{
			while (!stopped)
			{
				if (enableDetect)
				{
					StreamPacketPtr pkt{ imageDataGroup.front() };

					if (pkt)
					{
						cvPhoneAlgo.MainProcFunc((unsigned char*)pkt->getStreamData(), feedbackPhone);

						typedef std::map<int, StruMemoryInfo>::iterator Iterator;
						for (Iterator it = feedbackPhone.mapMemory.begin(); it != feedbackPhone.mapMemory.end();)
						{
//							if (iter->second.bDone)
							{
								// 计算最大的detectConfidence 和 fTrackConfidence;
								int nPhoneNum = 0;
								int nZuojiNum = 0;
								float maxConfidence = 0.f;
								int nSaveId = 0;
								for (int i = 0; i < it->second.vecSaveMat.size(); i++)
								{
									// 增加第二次识别过滤--对第一次检测的结果进行扩张后，进行第二次确认
									cv::Mat extractMat(
										static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_HEIGHT), 
										static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_WIDTH),
										CV_8UC3, 
										it->second.vecSaveMat[i].pUcharImage);
									cv::Rect expandRect;
									expandRect.x = (cv::max)(0, int(it->second.vecSaveMat[i].rRect.x - it->second.vecSaveMat[i].rRect.width * 0.1));
									expandRect.y = (cv::max)(0, int(it->second.vecSaveMat[i].rRect.y - it->second.vecSaveMat[i].rRect.height * 0.1));
									expandRect.width = (cv::min)(extractMat.cols - expandRect.x, int(it->second.vecSaveMat[i].rRect.width * 1.2));
									expandRect.height = (cv::min)(extractMat.rows - expandRect.y, int(it->second.vecSaveMat[i].rRect.height * 1.2));

									if (0 > expandRect.x || 0 > expandRect.y || 0 > expandRect.width || 0 > expandRect.height)
									{
										continue;
									}

									cv::Mat roiMat = extractMat(expandRect).clone();
									std::vector<StruDetectResult> secondResult;
//									nowProcTime = GetTickCount64();
									cvPhoneGpuDetect.MultiObjectDetect(roiMat, secondResult);
//									printf("=====  Phone.MultiObjectDetect run time = %lld.\r\n", GetTickCount64() - nowProcTime);

									float widthScale2 = (float)(roiMat.cols * 1.0 / 416);
									float heightScale2 = (float)(roiMat.rows * 1.0 / 416);
									if (!secondResult.empty())
									{
										if (secondResult[0].nLabel == 0)
										{
											nPhoneNum++;
										}
										else if (secondResult[0].nLabel == 1)
										{
											nZuojiNum++;
										}
									}
									else
										continue;

									if (it->second.vecSaveMat[i].detectConfidence > maxConfidence && secondResult[0].nLabel == 0)
									{
										maxConfidence = it->second.vecSaveMat[i].detectConfidence;
										nSaveId = i;
									}
								}

								//			if (nPhoneNum > nZuojiNum && 50 < iter->second.vecSaveMat[nSaveId].rRect.width) // 说明此组数据属于打电话，非座机，保存
								if (nPhoneNum > 0 || nZuojiNum > 0)
								{
									pkt->setAlarmRange(
										it->second.vecSaveMat[nSaveId].rRect.x,
										it->second.vecSaveMat[nSaveId].rRect.y,
										it->second.vecSaveMat[nSaveId].rRect.width,
										it->second.vecSaveMat[nSaveId].rRect.height);
									abstractFilter.afterProcessInputDataNotification(pkt);
								}

								for (int i = 0; i < it->second.vecSaveMat.size(); i++)
								{
									if (nullptr != it->second.vecSaveMat[i].pUcharImage)
										delete[] it->second.vecSaveMat[i].pUcharImage;
								}
								it = feedbackPhone.mapMemory.erase(it);
							}
						}
					}
					else
					{
						enableDetect = false;
					}

					imageDataGroup.popFront();
				}
				else
				{
					boost::this_thread::sleep(boost::posix_time::milliseconds(1));
				}
			}
		}
	}//namespace ai
}//namespace base
