//
//		Copyright :					@2018, HHJT, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2018-06-28
//		Description:				TCP服务端
//
//		History:						Author									Date													Description
//											王科威										2018-06-28									创建
//

#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "boost/shared_ptr.hpp"
#include "ASIO/ASIOService.h"
using ASIOServicePtr = boost::shared_ptr<NS(asio, 2)::ASIOService>;

NS_BEGIN(asio, 2)

class TCPServer
{
public:
	TCPServer(void);
	virtual ~TCPServer(void);

	Int32 startServer(const UInt16 port = 60531);
	Int32 stopServer(void);

protected:
	virtual void acceptingConnect(boost::asio::ip::tcp::socket* so = nullptr, const Int32 error = 0) = 0;

private:
	void afterRemoteConnectedNotificationCallback(boost::asio::ip::tcp::socket* so = nullptr, const Int32 error = 0);

private:
	ASIOServicePtr asioServicePtr;
};//class TCPServer

NS_END

#endif//TCP_SERVICE_H
