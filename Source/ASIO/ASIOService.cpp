#include "Hardware/Cpu.h"
using CPU = NS(hardware, 1)::Cpu;
#include "ASIO/ASIOService.h"

NS_BEGIN(asio, 2)

ASIOService::ASIOService() : idleNum{ 0 }
{}

ASIOService::~ASIOService()
{}

Int32 ASIOService::startService(const UInt8 workers /* = 1 */)
{
	Int32 status{ ERR_INVALID_PARAM };

	if (0 < workers)
	{
		status = startingService(workers);
	}

	return status;
}

void ASIOService::stopService()
{
	if (0 < ioContexts.size() && 0 < ioWorks.size())
	{
		stoppingService();
	}
}

Int32 ASIOService::startingService(const UInt8 workers /* = 1 */)
{
	IOContexts tempIOContexts(workers);

	for (UInt8 i = 0; i != workers; ++i)
	{
		boost::thread* tempWorkerThread{
			threadGroup.create_thread(boost::bind(&boost::asio::io_service::run, boost::ref(tempIOContexts[i]))) };
		CPU().setThreadAffinityMask(tempWorkerThread->native_handle(), i);
		ioWorks.push_back(boost::asio::io_service::work(tempIOContexts[i]));
	}

	ioContexts.swap(tempIOContexts);
	return 0 < ioContexts.size() ? ERR_OK : ERR_BAD_ALLOC;
}

void ASIOService::stoppingService()
{
	std::for_each(ioContexts.begin(), ioContexts.end(), boost::bind(&boost::asio::io_service::stop, _1));
	threadGroup.join_all();
	ioWorks.clear();
	ioContexts.clear();
}

boost::asio::io_service& ASIOService::getIdle()
{
	WriteLock tempWriteLock(idleMtx);
	return ioContexts.at(idleNum++ % ioContexts.size());
}

NS_END
