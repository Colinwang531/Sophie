//
//		Copyright :					@2017, HHJT, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-11-29
//		Description :				ASIO服务
//
//		History:						Author									Date													Description
//											王科威										2017-11-29									创建
//

#ifndef ASIO_SERVICE_H
#define ASIO_SERVICE_H

#include "boost/asio.hpp"
using IOContexts = std::vector<boost::asio::io_service>;
using IOWorks = std::vector<boost::asio::io_service::work>;
#include "boost/thread.hpp"
#include "Shared/SharedLock.h"
#include "Defs.h"

NS_BEGIN(asio, 2)

class ASIOService
{
public:
	ASIOService(void);
	virtual ~ASIOService(void);

	Int32 startService(const UInt8 workers = 1);
	void stopService(void);
	//	功能 : 使用循环遍历方法获取空闲的io_service实例
	//
	//	参数 :
	//
	//	返回值 : 必须是io_service&,因为io_service实例不能拷贝构造
	//
	//	备注 :
	//
	boost::asio::io_service& getIdle(void);

protected:
	virtual Int32 startingService(const UInt8 workers = 1);
	virtual void stoppingService(void);

protected:
	IOContexts ioContexts;
	IOWorks ioWorks;
	boost::thread_group threadGroup;
	UInt8 idleNum;
	SharedMutex idleMtx;
};//class ASIOService

NS_END

#endif//ASIO_SERVICE_H
