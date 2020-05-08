#ifdef WINDOWS
#include <windows.h>
#include "boost/winapi/system.hpp"
#else
#include <sys/sysinfo.h>
#include <sched.h>
#include <pthread.h>
#endif//WINDOWS
#include "Hardware/Cpu.h"

namespace base
{
	namespace hardware
	{
		Cpu::Cpu(){}
		Cpu::~Cpu(){}

		const int Cpu::getCPUCoreNumber() const
		{
			int tempNumberOfCores = 1;

#ifdef WINDOWS
			boost::winapi::_SYSTEM_INFO info;
			boost::winapi::GetSystemInfo(&info);
			tempNumberOfCores = info.dwNumberOfProcessors;
#else
			tempNumberOfCores = get_nprocs();
#endif//WINDOWS

			return tempNumberOfCores;
		}

		int Cpu::setThreadAffinityMask(
#ifdef WINDOWS
			void* tid /* = nullptr */,
#else
			unsigned long long tid /*= 0*/,
#endif//WINDOWS
			const int mask /* = 0 */)
		{
			int status{ 1 };

#ifdef WINDOWS
			const int tempThreadAffinityMask{ 0x01 << mask };
			status = static_cast<int>(SetThreadAffinityMask(tid, tempThreadAffinityMask));
#else
			cpu_set_t tempCpuSet;
			CPU_ZERO(&tempCpuSet);
			CPU_SET(mask, &tempCpuSet);
			status = pthread_setaffinity_np(tid, sizeof(cpu_set_t), &tempCpuSet);
#endif//WINDOWS

			return status;
		}
	}//namespace hardware
}//namespace base
