/****************************************************************************************************************************************************
Copyright :				@2017, HHJT, All Rights Reserved

Author :					������
E-mail :					wangkw531@gmail.com
Date :						2017-06-21
Description :			UDP���������

History:					Author									Date												Description
								������									2017-06-21								����
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

		//���෽��ʵ��
		virtual boost::uint16_t nextPortNum(void);

	private:
		boost::uint16_t startPortNum;
		Mutex portNumMtx;
	};//class UDPService
}//namespace network_1

#endif//UDP_SERVICE_H
