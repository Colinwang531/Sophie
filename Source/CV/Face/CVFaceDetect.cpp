#include "boost/bind.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/format.hpp"
#include "boost/pointer_cast.hpp"
#include "boost/thread.hpp"
#include "CV/FaceClass/include/BaseFaceDll.h"
#include "Define.h"
#include "Error.h"
#include "Thread/ThreadPool.h"
using ThreadPool = framework::thread::ThreadPool;
#include "Data/Protocol/ProtocolCrew.h"
using ProtocolCrew = framework::data::ProtocolCrew;
#include "Data/Frame/Frame.h"
using Frame = framework::data::Frame;
using FramePtr = boost::shared_ptr<Frame>;
#include "CV/Face/CVFaceDetect.h"

CVFaceDetect::CVFaceDetect(
	CVResultNotificationCallback callback /* = nullptr */)
	: face{ nullptr }, flag{ false }, cvresultNotificationCallback{ callback }, faceRegisterNumber{ 1 }
{}
CVFaceDetect::~CVFaceDetect()
{}

int CVFaceDetect::setFaceImage(void* param /* = nullptr */)
{
	int e{ param ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		using CrewMsg = framework::data::CrewMsg;
		CrewMsg* crew{ reinterpret_cast<CrewMsg*>(param) };
		const std::string filePath{
			boost::filesystem::initial_path<boost::filesystem::path>().string() };

		for (int i = 0; i != crew->crews.size(); ++i)
		{
			using CrewData = framework::data::CrewData;
			CrewData cd{ crew->crews[i] };

			const std::string imageFilePath{
				(boost::format("%s/Face/%s.jpg") % filePath % cd.id).str() };
			FILE* fd{ nullptr };
#ifdef WINDOWS
			fopen_s(&fd, imageFilePath.c_str(), "wb+");
#else
			fd = fopen(imageFilePath.c_str(), "wb+");
#endif//WINDOWS

			if (fd)
			{
				fwrite(cd.pictures[0].c_str(), cd.pictures[0].length(), 1, fd);
				fclose(fd);

				CFaceClass* fcp{ reinterpret_cast<CFaceClass*>(face) };
				if (fcp->RegisterFace(const_cast<char*>(imageFilePath.c_str()), faceRegisterNumber))
				{
					faces.insert(faceRegisterNumber, cd.id);
					faceRegisterNumber++;
				}
			}
		}
	}

	return e;
}

int CVFaceDetect::init(void* param /* = nullptr */)
{
	int e{ param ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		CVParam* cvp{ reinterpret_cast<CVParam*>(param) };
		const std::string filePath{
			boost::filesystem::initial_path<boost::filesystem::path>().string() };
		const std::string cfgFilePath{
			(boost::format("%s/model/haarcascade_frontalface_alt2") % filePath).str() };
// 		const std::string weightFilePath{
// 			(boost::format("%s/model/helmet_win.weights") % filePath).str() };

		StruInitParams initParams;
		initParams.gpu_id = cvp->gpuID;
		initParams.detectThreshold = cvp->detectThreshold;
		initParams.trackThreshold = cvp->trackThreshold;
		initParams.savePath = (char*)filePath.c_str();
		initParams.cfgfile = (char*)cfgFilePath.c_str();
		initParams.weightFile = "";//(char*)weightFilePath.c_str();
		initParams.matchThreshold = cvp->matchThreshold;
		ownID.append(cvp->ownID);

		CFaceClass* fp{ new(std::nothrow) CFaceClass };
		if (fp)
		{
			fp->InitModel(1920, 1080, initParams);
			face = fp;
			ThreadPool::get_mutable_instance().createNewThread(
				boost::bind(&CVFaceDetect::processingCalculateData, this));
		}
	}

	return e;
}

int CVFaceDetect::inputData(DataPtr data)
{
	int e{ data ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		if (face && !flag)
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

void CVFaceDetect::processingCalculateData()
{
	CFaceClass* fcp{ reinterpret_cast<CFaceClass*>(face) };

	while (fcp)
	{
		FeedBackFaceRecog fb;

		if (flag)
		{
			DataPtr data{ fifo.front() };

			if (data)
			{
				fcp->MainProcFunc((unsigned char*)(data->getData()), fb);
				FramePtr fp{
					boost::dynamic_pointer_cast<Frame>(data) };
				fp->setFrameID(ownID);

				if (fcp->PostProcessFunc(fb))
				{
					for (int j = 0; j != fb.vecFaceResult.size(); ++j)
					{
						const std::string userID{ faces.at(fb.vecFaceResult[j].matchId) };
						if (!userID.empty() && cvresultNotificationCallback)
						{
							fp->faceID = userID;
							cvresultNotificationCallback(data);
						}

						boost::checked_array_delete(fb.vecFaceResult[j].pUcharImage);
					}

					fb.vecFaceResult.clear();
				}

				fb.vecShowInfo.clear();
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(1));
			}
		}
	}
}