#include "boost/bind.hpp"
#include "boost/format.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "Error.h"
#include "Xml/XmlCodec.h"
using XMLParser = base::xml::XMLParser;
using XMLPacker = base::xml::XMLPacker;
#include "Time/XTime.h"
using Time = base::time::Time;
#include "Thread/ThreadPool.h"
using ThreadPool = base::thread::ThreadPool;
#include "Network/AbstractWorker.h"

namespace base
{
	namespace network
	{
		AbstractWorker::AbstractWorker() : AbstractClient()
		{}
		AbstractWorker::~AbstractWorker()
		{}

		void AbstractWorker::generateUUIDWithName(const std::string name)
		{
			const std::string item{ (boost::format("Component.%s.ID") % name).str() };
			XMLParser().getValueByName("Config.xml", item, uuid);

			if (uuid.empty())
			{
				uuid.append(boost::uuids::to_string(boost::uuids::random_generator()()));
				XMLPacker().setValueWithName("Config.xml", item, uuid);
			}
		}

		int AbstractWorker::createNewClient(const std::string)
		{
			return ThreadPool::get_mutable_instance().createNewThread(
				boost::bind(&AbstractWorker::processTaskTimerThread, this)) ? eSuccess : eBadNewThread;
		}

		void AbstractWorker::sendQuerySystemServiceMessage()
		{}

		void AbstractWorker::processTaskTimerThread()
		{
			//起始时间设为0保证第一次时间判断就执行注册
			long long startTime{ 0 };

			while (!AbstractClient::isStopped())
			{
				long long currentTime{ Time().tickcount() };
				if (30000 < currentTime - startTime)
				{
					startTime = currentTime;
					sendRegisterWorkerServerMessage();
					sendQuerySystemServiceMessage();
				}

				boost::this_thread::sleep(boost::posix_time::seconds(1));
			}
		}
	}//namespace network
}//namespace base
