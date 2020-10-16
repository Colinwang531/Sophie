//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-06
//		Description :					ASIO������
//
//		History:						Author									Date										Description
//										������									2020-05-06									����
//

#ifndef BASE_NETWORK_ASIO_SERVICE_H
#define BASE_NETWORK_ASIO_SERVICE_H

#include "boost/asio.hpp"
using IOCtxGroup = std::vector<boost::asio::io_service>;
using IOWorkGroup = std::vector<boost::asio::io_service::work>;
#include "boost/thread.hpp"
#include "Locker/Locker.h"
using SharedMutex = base::locker::SharedMutex;

namespace base
{
	namespace network
	{
		class ASIOService
		{
		public:
			ASIOService(void);
			virtual ~ASIOService(void);

		public:
			//����ASIO����
			//@Return : ������ֵ
			int startService(void);

			//ֹͣASIO����
			void stopService(void);
			
			//��ȡ����IO����ʵ��
			//@Return : ����IO����ʵ��
			boost::asio::io_service& getIdle(void);

		protected:
			IOCtxGroup ioctxGroup;
			IOWorkGroup ioWorkGroup;
			boost::thread_group threadGroup;
			int idleNumber;
			SharedMutex mtx;
		};//class ASIOService
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_ASIO_SERVICE_H
