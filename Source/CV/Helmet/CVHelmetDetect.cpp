#include "boost/bind.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/format.hpp"
#include "boost/pointer_cast.hpp"
#include "boost/thread.hpp"
#include "CV/HelmetClass/include/BaseHelmetDll.h"
#include "Define.h"
#include "Error.h"
#include "Thread/ThreadPool.h"
using ThreadPool = framework::thread::ThreadPool;
#include "Data/Frame/Frame.h"
using Frame = framework::data::Frame;
using FramePtr = boost::shared_ptr<Frame>;
#include "CV/Helmet/CVHelmetDetect.h"

CVHelmetDetect::CVHelmetDetect(
	CVResultNotificationCallback callback /* = nullptr */)
	: helmet{ nullptr }, flag{ false }, cvresultNotificationCallback{ callback }
{}
CVHelmetDetect::~CVHelmetDetect()
{}

int CVHelmetDetect::init(void* param /* = nullptr */)
{
	int e{ param ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		CVParam* cvp{ reinterpret_cast<CVParam*>(param) };
		const std::string filePath{
			boost::filesystem::initial_path<boost::filesystem::path>().string() };
		const std::string cfgFilePath{
			(boost::format("%s/model/helmet_win.cfg") % filePath).str() };
		const std::string weightFilePath{
			(boost::format("%s/model/helmet_win.weights") % filePath).str() };

		StruInitParams initParams;
		initParams.gpu_id = cvp->gpuID;
		initParams.detectThreshold = cvp->detectThreshold;
		initParams.trackThreshold = cvp->trackThreshold;
		initParams.savePath = (char*)filePath.c_str();
		initParams.cfgfile = (char*)cfgFilePath.c_str();
		initParams.weightFile = (char*)weightFilePath.c_str();
		ownID.append(cvp->ownID);

		CHelmetClass* hp{ new(std::nothrow) CHelmetClass };
		if (hp)
		{
			hp->InitAlgoriParam(1920, 1080, initParams);
			helmet = hp;
			ThreadPool::get_mutable_instance().createNewThread(
				boost::bind(&CVHelmetDetect::processingCalculateData, this));
		}
	}

	return e;
}

int CVHelmetDetect::inputData(DataPtr data)
{
	int e{ data ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		if (helmet && !flag)
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

void CVHelmetDetect::processingCalculateData()
{
	FeedBackHelmet fb;
	CHelmetClass* chc{ reinterpret_cast<CHelmetClass*>(helmet) };

	while (chc)
	{
		if (flag)
		{
			DataPtr data{ fifo.front() };

			if (data)
			{
				chc->MainProcFunc((unsigned char*)(data->getData()), fb);

				typedef std::map<int, StruMemoryInfo>::iterator Iterator;
				for (Iterator it = fb.mapMemory.begin(); it != fb.mapMemory.end();)
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
					{
						FramePtr fp{
							boost::dynamic_pointer_cast<Frame>(data) };
						framework::data::OTP pos{
							it->second.vecSaveMat[nSaveId].rRect.x,
							it->second.vecSaveMat[nSaveId].rRect.y,
							it->second.vecSaveMat[nSaveId].rRect.width,
							it->second.vecSaveMat[nSaveId].rRect.height };
						fp->addOT(pos);
						fp->setFrameID(ownID);

						if (cvresultNotificationCallback)
						{
							cvresultNotificationCallback(data);
						}
					}

					for (int i = 0; i < it->second.vecSaveMat.size(); i++)
					{
						if (nullptr != it->second.vecSaveMat[i].pUcharImage)
							delete[] it->second.vecSaveMat[i].pUcharImage;
					}

					it = fb.mapMemory.erase(it);
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
