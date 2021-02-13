//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-15
//		Description :					��ʱ����
//										��TCP�Ựʵ��ʹ��
//
//		History:						Author									Date										Description
//										������									2020-09-15									����
//

#ifndef FRAMEWORK_NETWORK_ASIO_TIMER_H
#define FRAMEWORK_NETWORK_ASIO_TIMER_H

#include "boost/enable_shared_from_this.hpp"
#include "boost/function.hpp"
#include "boost/system/error_code.hpp"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			typedef boost::function<void(void)> AfterTimerExpireResultCallback;

			class Timer 
				: public boost::enable_shared_from_this<Timer>
			{
			public:
				Timer(AfterTimerExpireResultCallback callback = nullptr);
				virtual ~Timer(void);

			public:
				//���ö�ʱ��
				//@s : socket���
				//@expire : ��ʱ���,����Ϊ��λ
				//@Return : ������ֵ
				int setTime(
					const void* s = nullptr,
					const int seconds = 5);

			private:
				//��ʱ����ʱ֪ͨ
				//@e : ������
				void afterTimerExpireResultNotification(const boost::system::error_code e);

			private:
				AfterTimerExpireResultCallback afterTimerExpireResultCallback;
			};//class Timer
		}//namespace asio
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ASIO_TIMER_H
