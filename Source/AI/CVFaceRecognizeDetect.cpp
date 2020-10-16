#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/format.hpp"
#include "boost/thread.hpp"
#include "Error.h"
#include "AI/CVFaceRecognizeDetect.h"

namespace base
{
	namespace ai
	{
		CVFaceRecognizeDetect::CVFaceRecognizeDetect(
			AbstractFilter& filter, 
			const AlgorithmType type /* = AlgorithmType::ALGORITHM_TYPE_ATTENDANCE_IN */)
			: AbstractAlgorithm(type), abstractFilter{ filter }, enableDetect{ false }
		{
			InitializeCriticalSection(&cs);
		}

		CVFaceRecognizeDetect::~CVFaceRecognizeDetect()
		{
			DeleteCriticalSection(&cs);
		}

		int CVFaceRecognizeDetect::addNewFaceImage(
			const int uid /* = -1 */,
			const char* image /* = nullptr */,
			const int bytes /* = 0 */)
		{
			int e{ -1 < uid && image && 0 < bytes ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				const std::string filePath{
					boost::filesystem::initial_path<boost::filesystem::path>().string() };
				const std::string imageFilePath{
					(boost::format("%s\\Face\\%d.jpg") % filePath % uid).str() };

				FILE* fd{ nullptr };
#ifdef WINDOWS
				fopen_s(&fd, imageFilePath.c_str(), "wb+");
#else
				fd = fopen(imageFilePath.c_str(), "wb+");
#endif//WINDOWS

				if (fd)
				{
					fwrite(image, bytes, 1, fd);
					fclose(fd);
					e = cvFaceRecognizeAlgo.RegisterFace(const_cast<char*>(imageFilePath.c_str()), uid) ? eSuccess : eBadOperate;
				}
				else
				{
					e = eBadOperate;
				}
			}

			return e;
		}

		int CVFaceRecognizeDetect::createNewAlgorithm(void* param /* = nullptr */)
		{
			AbstractAlgorithm* aa{ reinterpret_cast<AbstractAlgorithm*>(param) };
			int e{ aa ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				const std::string filePath{
					boost::filesystem::initial_path<boost::filesystem::path>().string() };
				const std::string cfgFilePath{ 
					(boost::format("%s\\model\\face.cfg") % filePath).str() };
				const std::string weightFilePath{ 
					(boost::format("%s\\model\\face.weights") % filePath).str() };

				StruInitParams initParams;
				initParams.gpu_id = aa->getGpuID() - 1;
				initParams.detectThreshold = aa->getFirstDetectThreshold();
				initParams.trackThreshold = aa->getTrackThreshold();
				initParams.savePath = (char*)filePath.c_str();
				initParams.cfgfile = (char*)cfgFilePath.c_str();
				initParams.weightFile = (char*)weightFilePath.c_str();

				e = cvFaceRecognizeAlgo.InitModel(
					static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_WIDTH),
					static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_HEIGHT),
					static_cast<int>(AlgorithmImageParam::ALGORITHM_IMAGE_CHANNEL),
					initParams,
					&cs) ? AbstractAlgorithm::createNewAlgorithm() : eBadOperate;
			}

			return e;
		}

		int CVFaceRecognizeDetect::destroyAlgorithm()
		{
			return AbstractAlgorithm::destroyAlgorithm();
		}

		int CVFaceRecognizeDetect::inputData(StreamPacketPtr pkt)
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

		void CVFaceRecognizeDetect::processingCalculateData()
		{
			while (!stopped)
			{
				if (enableDetect)
				{
					StreamPacketPtr pkt{ imageDataGroup.front() };

					if (pkt)
					{
						cvFaceRecognizeAlgo.MainProcFunc((unsigned char*)pkt->getStreamData(), feedbackFaceRecognize);

						if (cvFaceRecognizeAlgo.PostProcessFunc(feedbackFaceRecognize))
						{
							for (int j = 0; j != feedbackFaceRecognize.vecFaceResult.size(); ++j)
							{
								pkt->setAlarmRange(
									(int)(feedbackFaceRecognize.vecFaceResult[j].similarValue * 100),
									static_cast<int>(getAlgorithmType()),
									feedbackFaceRecognize.vecFaceResult[j].matchId);
								abstractFilter.afterProcessInputDataNotification(pkt);
								boost::checked_array_delete(feedbackFaceRecognize.vecFaceResult[j].pUcharImage);
							}

							feedbackFaceRecognize.vecFaceResult.clear();
						}

						feedbackFaceRecognize.vecShowInfo.clear();
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
