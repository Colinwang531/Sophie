//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-15
//		Description :					ʱ����
//
//		History:						Author									Date										Description
//										������									2020-05-26									����
//

#ifndef TIME_H
#define TIME_H

namespace base
{
	namespace time
	{
		class Time
		{
		public:
			Time(void);
			virtual ~Time(void);

		public:
			//��ȡϵͳʱ���,��ʼʱ��Ϊ1970-1-1 0��0��0
			//@Return : �Ժ���Ϊ��λ��ʱ�䳤��
			const long long tickcount(void);
		};//class Time
	}//namespace time
}//namespace base

#endif//TIME_H
