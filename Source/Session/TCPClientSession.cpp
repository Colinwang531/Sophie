#include "MediaSession/TCPClientSession.h"

NS_BEGIN(session, 2)

TCPClientSession::TCPClientSession(AfterReadDataNotificationCallback callback /* = nullptr */)
	: TCPSession(), afterReadDataNotificationCallback{ callback }
{}

TCPClientSession::~TCPClientSession()
{}

void TCPClientSession::receivingData(
	boost::system::error_code error, std::size_t transferBytes, const char* transferData /* = nullptr */)
{
	if (afterReadDataNotificationCallback)
	{
		afterReadDataNotificationCallback(tcpSocket, error.value(), transferData, transferBytes);
	}

	TCPSession::receivingData(error, transferBytes, transferData);
}

void TCPClientSession::settingTimeout(const UInt32 timeo /*= 0*/)
{
	if (0 < timeo && tcpSocket)
	{
		boost::posix_time::seconds tempTimeoutSeconds{ timeo };
		TimerPtr tempTimeoutPtr{
			boost::make_shared<boost::asio::deadline_timer>(tcpSocket->get_io_service(), tempTimeoutSeconds) };

		if (tempTimeoutPtr)
		{
			tempTimeoutPtr->async_wait(
				boost::bind(
					&TCPReceiver::afterReadDataTimeoutNotificationCallback,
					boost::enable_shared_from_this<TCPReceiver>::shared_from_this(),
					tcpSocket,
					boost::asio::placeholders::error,
					tempTimeoutPtr));
		}
	}
}

NS_END
