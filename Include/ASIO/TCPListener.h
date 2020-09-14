//
//		Copyright :					@2017, HHJT, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-06-19
//		Description:				TCP����
//
//		History:						Author									Date													Description
//											������										2017-06-19									����
//

#ifndef TCP_LISTENER_H
#define TCP_LISTENER_H

#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/function.hpp"
#include "Defs.h"

NS_BEGIN(asio, 2)

typedef boost::function<void(boost::asio::ip::tcp::socket*, boost::system::error_code)> AfterRemoteConnectedNotificationCallback;

class TCPListener : protected boost::enable_shared_from_this<TCPListener>
{
public:
	TCPListener(AfterRemoteConnectedNotificationCallback callback = nullptr);
	virtual ~TCPListener(void);

	//	���� : ��������
	//
	//	���� : 
	//			  @service [IN] io_serviceʵ��
	//			  @portNum [IN] �����˿�
	//
	//	����ֵ : ������
	//
	//	��ע :
	//
	Int32 asyncAccept(boost::asio::io_service& service, const UInt16 portNum = 60531);

private:
	void afterAsyncAcceptNotificationCallback(
		boost::asio::ip::tcp::socket* so, 
		boost::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor,
		boost::system::error_code error);

private:
	AfterRemoteConnectedNotificationCallback afterRemoteConnectedNotificationCallback;
};//class TCPListener

NS_END

#endif//TCP_LISTENER_H
