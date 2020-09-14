#include "boost/bind.hpp"
#include "boost/checked_delete.hpp"
#include "boost/functional/factory.hpp"
#include "boost/make_shared.hpp"
#include "ASIO/TCPListener.h"

NS_BEGIN(asio, 2)

TCPListener::TCPListener(AfterRemoteConnectedNotificationCallback callback /* = nullptr */)
	: afterRemoteConnectedNotificationCallback{ callback }
{}

TCPListener::~TCPListener()
{}

Int32 TCPListener::asyncAccept(boost::asio::io_service& service, const UInt16 portNum /* = 60531 */)
{
	Int32 status{ ERR_INVALID_PARAM };

	if (!service.stopped())
	{
		boost::asio::ip::tcp::socket* tempTCPSocket{ boost::factory<boost::asio::ip::tcp::socket*>()(service) };
		boost::shared_ptr<boost::asio::ip::tcp::acceptor> tempTCPAcceptorPtr{
			boost::make_shared<boost::asio::ip::tcp::acceptor>(
				service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), portNum)) };

		if (tempTCPSocket && tempTCPAcceptorPtr)
		{
			//After the acceptor has been initialized, listen() is called to make the acceptor start listening.
			//Then async_accept() is called to accept the first connection attempt.
			//A socket has to be passed as a first parameter to async_accept(), which will be used to send and receive data on a new connection.
			tempTCPAcceptorPtr->listen();
			tempTCPAcceptorPtr->async_accept(*tempTCPSocket,
				boost::bind(
					&TCPListener::afterAsyncAcceptNotificationCallback,
					boost::enable_shared_from_this<TCPListener>::shared_from_this(),
					tempTCPSocket,
					tempTCPAcceptorPtr,
					boost::asio::placeholders::error));
			status = ERR_OK;
		}
		else
		{
			boost::checked_delete(tempTCPSocket);
			status = ERR_BAD_ALLOC;
		}
	}

	return status;
}

void TCPListener::afterAsyncAcceptNotificationCallback(
	boost::asio::ip::tcp::socket* so, boost::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor, boost::system::error_code error)
{
	if (!error)
	{
		boost::asio::ip::tcp::socket* tempTCPSocket{ 
			boost::factory<boost::asio::ip::tcp::socket*>()(acceptor->get_io_service()) };

		if (tempTCPSocket)
		{
			acceptor->async_accept(*tempTCPSocket,
				boost::bind(
					&TCPListener::afterAsyncAcceptNotificationCallback,
					boost::enable_shared_from_this<TCPListener>::shared_from_this(),
					tempTCPSocket,
					acceptor,
					boost::asio::placeholders::error));
		}
	}

	if (afterRemoteConnectedNotificationCallback)
	{
		afterRemoteConnectedNotificationCallback(so, error);
	}
}

NS_END
