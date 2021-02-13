//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-06-02
//		Description :					�̳߳���
//
//		History:						Author									Date										Description
//										������									2020-06-02									����
//

#ifndef FRAMEWORK_THREAD_THREAD_POOL_H
#define FRAMEWORK_THREAD_THREAD_POOL_H

#include "boost/function.hpp"
#include "boost/serialization/singleton.hpp"
#include "boost/thread.hpp"

namespace framework
{
	namespace thread
	{
		typedef boost::function<void(void)> Function;

		class ThreadPool :
			public boost::serialization::singleton<ThreadPool>
		{
		public:
			ThreadPool(void);
			virtual ~ThreadPool(void);

		public:
			//�������߳�ʵ��
			//@f : �̺߳���
			//@Return : �߳̾��ֵ
			void* createNewThread(Function f);

			//�����߳�
			//@t : �߳̾��ֵ
			void destroyThread(void* t = nullptr);

			//�̰߳�ȫ�˳�
			void shutdownAllThread(void);

		private:
			boost::thread_group tg;
		};//class ThreadPool
	}//namespace thread
}//namespace framework

#endif//FRAMEWORK_THREAD_THREAD_POOL_H
