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
//										������									2020-11-25									�޸�ʱ�������ֵΪ�޷�������
//

#ifndef FRAMEWORK_TIME_TIME_H
#define FRAMEWORK_TIME_TIME_H

namespace framework
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
			const unsigned long long tickcount(void);

			//ִ��˯��
			//@interval : ˯��ʱ��,����Ϊ��λ
			void sleep(const unsigned int interval = 1);
		};//class Time
	}//namespace time
}//namespace framework

#endif//FRAMEWORK_TIME_TIME_H
