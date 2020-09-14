#include "boost/bind.hpp"
#include "boost/checked_delete.hpp"
#include "ASIO/TCPConnector.h"

NS_BEGIN(asio, 2)

TCPConnector::TCPConnector(
	boost::asio::ip::tcp::socket* so /* = nullptr */, AfterRemoteConnectNotificationCallback callback /* = nullptr */)
	: afterRemoteConnectNotificationCallback{ callback }, tcpSocket{ so }
{}

TCPConnector::~TCPConnector()
{}

void TCPConnector::asyncConnect(const char* address /* = nullptr */, const UInt16 port /* = 60531 */)
{
	if (tcpSocket)
	{
		tcpSocket->async_connect(
			boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(address), port),
			boost::bind(
				&TCPConnector::afterAsyncConnectNotificationCallback,
				boost::enable_shared_from_this<TCPConnector>::shared_from_this(),
				boost::asio::placeholders::error));
	}
}

void TCPConnector::afterAsyncConnectNotificationCallback(const boost::system::error_code error)
{
	const Int32 tempErrorCode{ error.value() };

	if (afterRemoteConnectNotificationCallback)
	{
		afterRemoteConnectNotificationCallback(tcpSocket, tempErrorCode);
	}

	if (tempErrorCode && tcpSocket)
	{
		tcpSocket->close();
		boost::checked_delete(tcpSocket);
	}
}

NS_END
