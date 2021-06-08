#ifdef WINDOWS
#include <windows.h>
#include "boost/winapi/system.hpp"
#else
#include <sys/sysinfo.h>
#include <sched.h>
#include <pthread.h>
#endif//WINDOWS
#include "error.h"
#include "cpu.h"

namespace framework
{
	namespace libcommon
	{
		namespace hardware
		{
			Cpu::Cpu(){}
			Cpu::~Cpu(){}

			const int Cpu::getCoreNumber() const
			{
				int number{1};

#ifdef WINDOWS
				boost::winapi::_SYSTEM_INFO info;
				boost::winapi::GetSystemInfo(&info);
				number = info.dwNumberOfProcessors;
#else
				number = get_nprocs();
#endif//WINDOWS

				return number;
			}

			int Cpu::setAffinity(
#ifdef WINDOWS
				void* tid /* = nullptr */,
#else
				unsigned long long tid /*= 0*/,
#endif//WINDOWS
				const int mask /* = 0 */)
			{
				int e{ 0 };

#ifdef WINDOWS
				const int maskNumber{ 0x01 << mask };
				e = (0 == SetThreadAffinityMask(tid, maskNumber) ? static_cast<int>(CommonError::COMMON_ERROR_SET_FAILED) : static_cast<int>(CommonError::COMMON_ERROR_SUCCESS));
#else
				cpu_set_t cs;
				CPU_ZERO(&cs);
				CPU_SET(mask, &cs);
				e = (0 == pthread_setaffinity_np(tid, sizeof(cpu_set_t), &cs) ? static_cast<int>(CommonError::COMMON_ERROR_SUCCESS) : static_cast<int>(CommonError::COMMON_ERROR_SET_FAILED));
#endif//WINDOWS

				return e;
			}
		}//namespace hardware
	}//namespace hardware
}//namespace base
