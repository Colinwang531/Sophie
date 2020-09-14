/****************************************************************************************************************************************************
Copyright :				@2017, HHJT, All Rights Reserved

Author :					王科威
E-mail :					wangkw531@gmail.com
Date :						2017-06-21
Description :			UDP网络服务类

History:					Author									Date												Description
								王科威									2017-06-21								创建
****************************************************************************************************************************************************/


#ifndef UDP_SERVICE_H
#define UDP_SERVICE_H

#include "GeneralService.h"

namespace network_1
{
	class UDPService : public GeneralService
	{
		typedef boost::mutex Mutex;
		typedef boost::lock_guard<Mutex> Locker;

	public:
		UDPService(void);
		virtual ~UDPService(void);

		//基类方法实现
		virtual boost::uint16_t nextPortNum(void);

	private:
		boost::uint16_t startPortNum;
		Mutex portNumMtx;
	};//class UDPService
}//namespace network_1

#endif//UDP_SERVICE_H
