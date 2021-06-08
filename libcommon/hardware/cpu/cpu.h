//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-04-26
//		Description :					CPU类
//
//		History:						Author									Date										Description
//										王科威									2020-04-26									 创建
//

#ifndef FRAMEWORK_LIBCOMMON_HARDWARE_CPU_H
#define FRAMEWORK_LIBCOMMON_HARDWARE_CPU_H

namespace framework
{
	namespace libcommon
	{
		namespace hardware
		{
			class Cpu
			{
			public:
				Cpu(void);
				~Cpu(void);

				//获取CPU核心数
				//@Return : CPU核心数
				const int getCoreNumber(void) const;

				//设置线程运行的CPU核心
				//@tid : 线程
				//@mask : CPU索引号
				//@Return : 错误码
				int setAffinity(
	#ifdef WINDOWS
					void* tid = nullptr,
	#else
					unsigned long long tid = 0,
	#endif//WINDOWS
					const int mask = 0);
			};//class Cpu
		}//namespace hardware
	}//namespace libcommon
}//namespace framework

#endif//FRAMEWORK_LIBCOMMON_HARDWARE_CPU_H
