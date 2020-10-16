#include "Hardware/Cpu.h"
using Cpu = base::hardware::Cpu;
#include "ASIO/ASIOService.h"
#include "Error.h"
using WriteLocker = base::locker::WriteLocker;

namespace base
{
	namespace network
	{
		ASIOService::ASIOService() : idleNumber{ 0 }
		{}

		ASIOService::~ASIOService()
		{}

		int ASIOService::startService()
		{
			const int cpuNumber{ Cpu().getCPUCoreNumber() };
			IOCtxGroup ctxs(cpuNumber);

			for (int i = 0; i != cpuNumber; ++i)
			{
				boost::thread* tempWorkerThread{
					threadGroup.create_thread(boost::bind(&boost::asio::io_service::run, boost::ref(ctxs[i]))) };
				Cpu().setThreadAffinityMask(tempWorkerThread->native_handle(), i);
				ioWorkGroup.push_back(boost::asio::io_service::work(ctxs[i]));
			}

			ioctxGroup.swap(ctxs);
			return 0 < ioWorkGroup.size() ? eSuccess : eBadOperate;
		}

		void ASIOService::stopService()
		{
			std::for_each(ioctxGroup.begin(), ioctxGroup.end(), boost::bind(&boost::asio::io_service::stop, _1));
			threadGroup.join_all();
			ioWorkGroup.clear();
			ioctxGroup.clear();
		}

		boost::asio::io_service& ASIOService::getIdle()
		{
			WriteLocker locker(mtx);
			return ioctxGroup.at(idleNumber++ % ioctxGroup.size());
		}
	}//namespace network
}//namespace base
