#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/format.hpp"
#include "boost/thread.hpp"
#include "Error.h"
#include "AI/CVSleepDetect.h"

namespace base
{
	namespace ai
	{
		CVSleepDetect::CVSleepDetect(AbstractFilter& filter)
			: AbstractAlgorithm(AlgorithmType::ALGORITHM_TYPE_SLEEP), abstractFilter{ filter }, enableDetect{ false }
		{}
		CVSleepDetect::~CVSleepDetect(){}

		int CVSleepDetect::createNewAlgorithm(void* param /* = nullptr */)
		{
			AbstractAlgorithm* aa{ reinterpret_cast<AbstractAlgorithm*>(param) };
			int e{ aa ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				const std::string filePath{
					boost::filesystem::initial_path<boost::filesystem::path>().string() };
				const std::string cfgFilePath{ 
					(boost::format("%s\\model\\helmet_sleep.cfg") % filePath).str() };
				const std::string weightFilePath{ 
					(boost::format("%s\\model\\helmet_sleep.weights") % filePath).str() };

				StruInitParams initParams;
				initParams.gpu_id = aa->getGpuID() - 1;
				initParams.detectThreshold = aa->getFirstDetectThreshold();
				initParams.trackThreshold = aa->getTrackThreshold();
				initParams.savePath = (char*)filePath.c_str();
				initParams.cfgfile = (char*)cfgFilePath.c_str();
				initParams.weightFile = (char*)weightFilePath.c_str();
				initParams.sleepTime = 240;

				e = cvSleepAlgo.InitAlgoriParam(
					static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_WIDTH),
					static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_HEIGHT),
					static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_CHANNEL),
					initParams) ? AbstractAlgorithm::createNewAlgorithm() : eBadOperate;
			}

			return e;
		}

		int CVSleepDetect::destroyAlgorithm()
		{
			return AbstractAlgorithm::destroyAlgorithm();
		}

		int CVSleepDetect::inputData(StreamPacketPtr pkt)
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

		void CVSleepDetect::processingCalculateData()
		{
			while (!stopped)
			{
				if (enableDetect)
				{
					StreamPacketPtr pkt{ imageDataGroup.front() };

					if (pkt)
					{
						cvSleepAlgo.MainProcFunc((unsigned char*)pkt->getStreamData(), feedbackSleep);

						typedef std::map<int, StruMemoryInfo>::iterator Iterator;
						for (Iterator it = feedbackSleep.mapMemory.begin(); it != feedbackSleep.mapMemory.end();)
						{
							if (1 == it->second.nCatch)
							{
								int nSaveId{ (int)(it->second.vecSaveMat.size() - 1) };
								it->second.nCatch = 2;

								pkt->setAlarmRange(
									it->second.vecSaveMat[nSaveId].rRect.x,
									it->second.vecSaveMat[nSaveId].rRect.y,
									it->second.vecSaveMat[nSaveId].rRect.width,
									it->second.vecSaveMat[nSaveId].rRect.height);
								abstractFilter.afterProcessInputDataNotification(pkt);

								printf("Sleep alarm x = %d, y = %d, w = %d, h = %d, label = %d.\r\n",
									it->second.vecSaveMat[nSaveId].rRect.x,
									it->second.vecSaveMat[nSaveId].rRect.y,
									it->second.vecSaveMat[nSaveId].rRect.width,
									it->second.vecSaveMat[nSaveId].rRect.height,
									it->second.vecSaveMat[nSaveId].nLabel);
								boost::checked_array_delete(it->second.vecSaveMat[nSaveId].pUcharImage);
							}

							if (it->second.bDone)
							{
								it = feedbackSleep.mapMemory.erase(it);
							}
							else
							{
								it++;
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
