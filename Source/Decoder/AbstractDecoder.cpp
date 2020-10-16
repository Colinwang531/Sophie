#include "Error.h"
#include "Thread/ThreadPool.h"
using ThreadPool = base::thread::ThreadPool;
#include "Decoder/AbstractDecoder.h"

namespace base
{
	namespace stream
	{
		AbstractDecoder::AbstractDecoder(AbstractFilter& filter) : abstractFilter{ filter }, stopped{ true }
		{}

		AbstractDecoder::~AbstractDecoder()
		{}

		int AbstractDecoder::createNewDecoder()
		{
			stopped = false;
			return ThreadPool::get_mutable_instance().createNewThread(
				boost::bind(&AbstractDecoder::processDecodeWorkerkThread, this)) ? eSuccess : eBadNewThread;
		}

		int AbstractDecoder::destroyDecoder()
		{
			stopped = true;
			return eSuccess;
		}

		void AbstractDecoder::processDecodeWorkerkThread()
		{
			while (!stopped)
			{
				processingDecodeData();
			}
		}
	}//namespace stream
}//namespace base
