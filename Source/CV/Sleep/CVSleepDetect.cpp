#include "boost/bind.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/format.hpp"
#include "boost/pointer_cast.hpp"
#include "boost/thread.hpp"
#include "CV/SleepClass/include/BaseSleepDll.h"
#include "Define.h"
#include "Error.h"
#include "Time/XTime.h"
using Time = framework::time::Time;
#include "Thread/ThreadPool.h"
using ThreadPool = framework::thread::ThreadPool;
#include "Data/Frame/Frame.h"
using Frame = framework::data::Frame;
using FramePtr = boost::shared_ptr<Frame>;
#include "CV/Sleep/CVSleepDetect.h"

CVSleepDetect::CVSleepDetect(
	CVResultNotificationCallback callback /* = nullptr */)
	: sleep{ nullptr }, flag{ false }, cvresultNotificationCallback{ callback }, lastObjectDetectTickcount{0}
{}
CVSleepDetect::~CVSleepDetect() {}

int CVSleepDetect::init(void* param /* = nullptr */)
{
	int e{ param ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		CVParam* cvp{ reinterpret_cast<CVParam*>(param) };
		const std::string filePath{
			boost::filesystem::initial_path<boost::filesystem::path>().string() };
		const std::string cfgFilePath{
			(boost::format("%s/model/helmet_sleep.cfg") % filePath).str() };
		const std::string weightFilePath{
			(boost::format("%s/model/helmet_sleep.weights") % filePath).str() };

		StruInitParams initParams;
		initParams.gpu_id = cvp->gpuID;
		initParams.detectThreshold = cvp->detectThreshold;
		initParams.trackThreshold = cvp->trackThreshold;
		initParams.savePath = (char*)filePath.c_str();
		initParams.cfgfile = (char*)cfgFilePath.c_str();
		initParams.weightFile = (char*)weightFilePath.c_str();
		initParams.sleepTime = cvp->sleepTime;
		ownID.append(cvp->ownID);

		CSleepClass* sp{ new(std::nothrow) CSleepClass };
		if (sp)
		{
			sp->InitAlgoriParam(1920, 1080, initParams);
			this->sleep = sp;
			ThreadPool::get_mutable_instance().createNewThread(
				boost::bind(&CVSleepDetect::processingCalculateData, this));
		}
	}

	return e;
}

int CVSleepDetect::inputData(DataPtr data)
{
	int e{ data ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		if (sleep && !flag)
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

void CVSleepDetect::processingCalculateData()
{
	FeedBackSleep fb;
	CSleepClass* sp{ reinterpret_cast<CSleepClass*>(this->sleep) };

	while (sp)
	{
		if (flag)
		{
			DataPtr data{ fifo.front() };

			if (data)
			{
				sp->MainProcFunc((unsigned char*)(data->getData()), fb);

				const unsigned long long nowTickcount{ Time().tickcount() };
				FramePtr fp{
					boost::dynamic_pointer_cast<Frame>(data) };
				fp->setFrameID(ownID);
				const int targetNumber{ (int)fb.vecShowInfo.size() };

				if (5000 < nowTickcount - lastObjectDetectTickcount && cvresultNotificationCallback)
				{
					for (int i = 0; i != targetNumber; ++i)
					{
						framework::data::OTP otp{
							fb.vecShowInfo[i].rRect.x,
							fb.vecShowInfo[i].rRect.y,
							fb.vecShowInfo[i].rRect.width,
							fb.vecShowInfo[i].rRect.height };
						fp->addOT(otp, true);
					}
					cvresultNotificationCallback(data);
					lastObjectDetectTickcount = nowTickcount;
				}

				fp->clearOT();
				typedef std::map<int, StruMemoryInfo>::iterator Iterator;
				for (Iterator it = fb.mapMemory.begin(); it != fb.mapMemory.end();)
				{
					if (1 == it->second.nCatch)
					{
						int nSaveId{ (int)(it->second.vecSaveMat.size() - 1) };
						it->second.nCatch = 2;

						FramePtr fp{ boost::dynamic_pointer_cast<Frame>(data) };
						framework::data::OTP otp{
							it->second.vecSaveMat[nSaveId].rRect.x,
							it->second.vecSaveMat[nSaveId].rRect.y,
							it->second.vecSaveMat[nSaveId].rRect.width,
							it->second.vecSaveMat[nSaveId].rRect.height };
						fp->addOT(otp);

						if (cvresultNotificationCallback)
						{
							cvresultNotificationCallback(data);
						}

						boost::checked_array_delete(it->second.vecSaveMat[nSaveId].pUcharImage);
					}

					if (it->second.bDone)
					{
						it = fb.mapMemory.erase(it);
					}
					else
					{
						it++;
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
