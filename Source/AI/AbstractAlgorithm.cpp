#include "Error.h"
#include "Thread/ThreadPool.h"
using ThreadPool = base::thread::ThreadPool;
#include "AI/AbstractAlgorithm.h"

namespace base
{
	namespace ai
	{
		AbstractAlgorithm::AbstractAlgorithm(
			const AlgorithmType type /* = AlgorithmType::ALGORITHM_TYPE_NONE */)
			: algorithmType{ type }, gpuID{ 0 }, firstDetectThreshold{ 0.0f },
			secondDetectThreshold{ 0.0f }, trackThreshold{ 0.0f }, similarThreashold{ 0.0f }, stopped{ false }
		{}
		AbstractAlgorithm::AbstractAlgorithm(const AbstractAlgorithm& algo)
			: algorithmType{ algo.getAlgorithmType() }, gpuID{ algo.getGpuID() }, firstDetectThreshold{ algo.getFirstDetectThreshold() },
			secondDetectThreshold{ algo.getSecondDetectThreshold() }, trackThreshold{ algo.getTrackThreshold() }, similarThreashold{ algo.getSimilarThreshold() }
		{}
		AbstractAlgorithm::~AbstractAlgorithm(){}

		int AbstractAlgorithm::createNewAlgorithm(void* param /* = nullptr */)
		{
			return ThreadPool::get_mutable_instance().createNewThread(
				boost::bind(&AbstractAlgorithm::processCalculateWorkerkThread, this)) ? eSuccess : eBadNewThread;
		}

		int AbstractAlgorithm::destroyAlgorithm()
		{
			stopped = true;
			return eNotSupport;
		}

		int AbstractAlgorithm::inputData(StreamPacketPtr pkt)
		{
			return pkt ? eSuccess : eInvalidParameter;
		}

		AbstractAlgorithm& AbstractAlgorithm::operator=(const AbstractAlgorithm& algo)
		{
			algorithmType = algo.algorithmType;
			gpuID = algo.gpuID;
			streamID = algo.streamID;
			firstDetectThreshold = algo.firstDetectThreshold;
			secondDetectThreshold = algo.secondDetectThreshold;
			trackThreshold = algo.trackThreshold;
			similarThreashold = algo.similarThreashold;
			return *this;
		}

		void AbstractAlgorithm::processCalculateWorkerkThread()
		{
			while (!stopped)
			{
				processingCalculateData();
				boost::this_thread::sleep(boost::posix_time::milliseconds(1));
			}
		}

		void AbstractAlgorithm::processingCalculateData() {}
	}//namespace ai
}//namespace base
