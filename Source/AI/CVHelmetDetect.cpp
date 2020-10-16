#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/format.hpp"
#include "boost/thread.hpp"
#include "Error.h"
#include "AI/CVHelmetDetect.h"

namespace base
{
	namespace ai
	{
		CVHelmetDetect::CVHelmetDetect(AbstractFilter& filter)
			: AbstractAlgorithm(AlgorithmType::ALGORITHM_TYPE_HELMET), abstractFilter{ filter }, enableDetect{ false }
		{}
		CVHelmetDetect::~CVHelmetDetect(){}

		int CVHelmetDetect::createNewAlgorithm(void* param /* = nullptr */)
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

				e = cvHelmetAlgo.InitAlgoriParam(
					static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_WIDTH),
					static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_HEIGHT),
					static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_CHANNEL),
					initParams) ? AbstractAlgorithm::createNewAlgorithm() : eBadOperate;
			}

			return e;
		}

		int CVHelmetDetect::destroyAlgorithm()
		{
			return AbstractAlgorithm::destroyAlgorithm();
		}

		int CVHelmetDetect::inputData(StreamPacketPtr pkt)
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

		void CVHelmetDetect::processingCalculateData()
		{
			while (!stopped)
			{
				if (enableDetect)
				{
					StreamPacketPtr pkt{ imageDataGroup.front() };

					if (pkt)
					{
						cvHelmetAlgo.MainProcFunc((unsigned char*)(pkt->getStreamData()), feedbackHelmet);

						typedef std::map<int, StruMemoryInfo>::iterator Iterator;
						for (Iterator it = feedbackHelmet.mapMemory.begin(); it != feedbackHelmet.mapMemory.end();)
						{
							int noneHelmetNumber{ 0 }, haveHelmetNumber{ 0 };
							float maxConfidence{ 0.0f };
							int nSaveId{ (int)(it->second.vecSaveMat.size() - 1) };

							for (int i = 0; i != it->second.vecSaveMat.size(); i++)
							{
								if (0 == it->second.vecSaveMat[i].nLabel)
								{
									noneHelmetNumber++;
								}
								else
								{
									haveHelmetNumber++;
								}
							}

							if (noneHelmetNumber > haveHelmetNumber && -1 < nSaveId)
// 								&&
// 								150 >= it->second.vecSaveMat[nSaveId].rRect.width &&
// 								150 >= it->second.vecSaveMat[nSaveId].rRect.height)
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

							it = feedbackHelmet.mapMemory.erase(it);
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
