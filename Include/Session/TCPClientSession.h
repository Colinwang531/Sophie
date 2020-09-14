//
//		Copyright :					@2018, HHJT, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2018-07-23
//		Description:				TCP�ͻ��˻Ự
//
//		History:						Author									Date													Description
//											������										2018-07-23									����
//

#ifndef TCP_CLIENT_SESSION_H
#define TCP_CLIENT_SESSION_H

#include "boost/function.hpp"
#include "ASIO/TCPSession.h"
using TCPSession = NS(asio, 2)::TCPSession;

NS_BEGIN(session, 2)

typedef boost::function<
	void(boost::asio::ip::tcp::socket* so, const Int32 error, const char* transferData, const Int32 transferBytes)> AfterReadDataNotificationCallback;

class TCPClientSession : public TCPSession
{
public:
	TCPClientSession(AfterReadDataNotificationCallback callback = nullptr);
	virtual ~TCPClientSession(void);

protected:
	void receivingData(
		boost::system::error_code error, std::size_t transferBytes, const char* transferData = nullptr) override;
	void settingTimeout(const UInt32 timeo = 0) override;

private:
	AfterReadDataNotificationCallback afterReadDataNotificationCallback;
};//class TCPClientSession

NS_END

#endif//TCP_CLIENT_SESSION_H
