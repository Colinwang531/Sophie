//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-04-26
//		Description :					Ӳ���豸֮CPU��
//
//		History:						Author									Date										Description
//										������									2020-04-26									����
//

#ifndef HW_CPU_H
#define HW_CPU_H

namespace hw
{
	namespace cpu
	{
		class Cpu
		{
		public:
			Cpu(void);
			virtual ~Cpu(void);

			//��ȡ�����CPU������
			const int getCPUCoreNumber(void) const;

			//�÷���Ӧ�������߳�����
			int setThreadAffinityMask(
#ifdef WINDOWS
				void* tid = nullptr,
#else
				unsigned long long tid = 0,
#endif//WINDOWS
				const int mask = 0);
		};//class Cpu
	}//namespace Cpu
}//namespace hw

#endif//HW_CPU_H
