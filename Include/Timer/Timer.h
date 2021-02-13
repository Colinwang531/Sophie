//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-15
//		Description :					定时器类
//										仅TCP会话实例使用
//
//		History:						Author									Date										Description
//										王科威									2020-09-15									创建
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
				//设置定时器
				//@s : socket句柄
				//@expire : 超时间隔,以秒为单位
				//@Return : 错误码值
				int setTime(
					const void* s = nullptr,
					const int seconds = 5);

			private:
				//定时器超时通知
				//@e : 错误码
				void afterTimerExpireResultNotification(const boost::system::error_code e);

			private:
				AfterTimerExpireResultCallback afterTimerExpireResultCallback;
			};//class Timer
		}//namespace asio
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ASIO_TIMER_H
