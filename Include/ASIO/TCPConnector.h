//
//		Copyright :					@2017, HHJT, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-07-14
//		Description:				TCP连接器
//
//		History:						Author									Date													Description
//											王科威										2017-07-14									创建
//

#ifndef TCP_CONNECTOR_H
#define TCP_CONNECTOR_H

#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/function.hpp"
#include "Defs.h"

NS_BEGIN(asio, 2)

typedef boost::function<void(boost::asio::ip::tcp::socket*, const Int32)> AfterRemoteConnectNotificationCallback;

class TCPConnector : public boost::enable_shared_from_this<TCPConnector>
{
public:
	TCPConnector(
		boost::asio::ip::tcp::socket* so = nullptr,
		AfterRemoteConnectNotificationCallback callback = nullptr);
	virtual ~TCPConnector(void);

	//	功能 : 异步连接远程主机
	//
	//	参数 : 
	//			  @address [IN] 主机IPv4地址
	//			  @port [IN] 主机端口号
	//
	//	返回值 :
	//
	//	备注 :
	//
	void asyncConnect(const char* address = nullptr, const UInt16 port = 60531);

private:
	//	功能 : 连接事件捕获通知回调函数
	//
	//	参数 : 
	//			  @error [IN] 网络错误码
	//
	//	返回值 :
	//
	//	备注 : 在使用async_connect方法进行异步连接时,捕获到的通知没有返回socket实例,因此无法将socket实例和连接事件对应起来
	//			  为能解决对应的问题,暂时使用了2个回调函数进行通知,后期可修改为协程的调用方式
	//
	void afterAsyncConnectNotificationCallback(const boost::system::error_code error);

private:
	boost::asio::ip::tcp::socket* tcpSocket;
	AfterRemoteConnectNotificationCallback afterRemoteConnectNotificationCallback;
};//class TCPConnector

NS_END

#endif//TCP_CONNECTOR_H
