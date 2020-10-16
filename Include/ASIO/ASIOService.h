//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-06
//		Description :					ASIO服务类
//
//		History:						Author									Date										Description
//										王科威									2020-05-06									创建
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
			//启动ASIO服务
			//@Return : 错误码值
			int startService(void);

			//停止ASIO服务
			void stopService(void);
			
			//获取空闲IO服务实例
			//@Return : 空闲IO服务实例
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
