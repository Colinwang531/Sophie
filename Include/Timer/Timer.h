//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-15
//		Description :					��ʱ����
//
//		History:						Author									Date										Description
//										������									2020-09-15									����
//

#ifndef BASE_NETWORK_TIMER_H
#define BASE_NETWORK_TIMER_H

#include "boost/asio.hpp"
#include "boost/function.hpp"
#include "boost/smart_ptr/enable_shared_from_this.hpp"
using DeadLineTimerPtr = boost::shared_ptr<boost::asio::deadline_timer>;

namespace base
{
	namespace network
	{
		typedef boost::function<void(void)> AfterGotTimeoutNotificationCallback;

		class Timer : public boost::enable_shared_from_this<Timer>
		{
		public:
			Timer(
				AfterGotTimeoutNotificationCallback callback = nullptr);
			virtual ~Timer(void);

			//�򿪶�ʱ��
			//@s : Socket���
			//@expire : ��ʱ���,����Ϊ��λ
			//@Return : ������ֵ
			int waitTimeout(
				boost::asio::ip::tcp::socket* s = nullptr, 
				const int expire = 5);

		private:
			void afterWaitTimeoutNotificationCallback(
				boost::system::error_code e);

		private:
			AfterGotTimeoutNotificationCallback afterGotTimeoutNotificationCallback;
		};//class Timer
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_TIMER_H
