#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "boost/thread/lock_guard.hpp"
#include "ASIO/TCPSession.h"

NS_BEGIN(asio, 2)

TCPSession::TCPSession() : tcpSocket{ nullptr }, stopped{ false }
{}

TCPSession::~TCPSession()
{}

Int32 TCPSession::openSession(boost::asio::ip::tcp::socket* so /* = nullptr */)
{
	if (!so)
	{
		return ERR_INVALID_PARAM;
	}

	if (tcpSocket)
	{
		return ERR_EXISTED;
	}

	tcpSocket = so;
	return ERR_OK;
}

void TCPSession::closeSession()
{
	closingSession();
	if (!sendingDataQueue.empty())
	{
		sendingDataQueue.clear();
	}
}

Int32 TCPSession::sendData(const MediaData& data)
{
	Int32 status{ ERR_BAD_OPERATE };

	if (Opened())
	{
		status = data.Valid() ? ERR_OK : ERR_INVALID_PARAM;

		if (ERR_OK == status)
		{
			sendingData(data);
		}
	}

	return status;
}

void TCPSession::receiveData(const UInt32 timeo /* = 0 */)
{
	//先读数据再设置超时时间
	tcpReceiver.asyncRead(tcpSocket, boost::bind(&TCPSession::afterAsyncReadDataNotificationCallback, this, _1, _2));
	settingTimeout(timeo);
}

void TCPSession::afterAsyncReadDataNotificationCallback(boost::system::error_code error, std::size_t transferBytes)
{
	receivingData(tcpReceiver.Data(), static_cast<Int32>(transferredBytes), error.value());
}

void TCPSession::afterAsyncSendDataNotificationCallback(const boost::system::error_code& error, const std::size_t transferredBytes)
{
//	TCPSession* tempTCPSession{ reinterpret_cast<TCPSession*>(ctx) };

	if (/*tempTCPSession && */!error)
	{
// 		if (tempTCPSession->tcpSenderPtr && so == tempTCPSession->tcpSocket)
// 		{
			boost::lock_guard<boost::mutex> tempLockGuard{ /*tempTCPSession->*/sendingDataMutex };
			/*tempTCPSession->*/sendingDataQueue.pop_front();
			if (!/*tempTCPSession->*/sendingDataQueue.empty())
			{
				const MediaData& tempMediaData{ /*tempTCPSession->*/sendingDataQueue.front() };
				tcpSender.sendData(
					tcpSocket, tempMediaData.Data(), tempMediaData.Bytes(), boost::bind(&TCPSession::afterAsyncSendDataNotificationCallback, this, _1, _2));
			}
//		}
	}
}

void TCPSession::sendingData(const MediaData& data)
{
// 	Int32 status{ ERR_INVALID_PARAM };
// 
// 	if (data)
// 	{
		boost::lock_guard<boost::mutex> tempLockGuard{ sendingDataMutex };
		const bool tempIsQueueEmpty{ sendingDataQueue.empty() };
		sendingDataQueue.push_back(data);

		if (tempIsQueueEmpty)
		{
// 			boost::shared_ptr<TCPSender> tempTCPSenderPtr{ 
// 				boost::make_shared<TCPSender>(&TCPSession::gotSendDataNotificationCallback, this) };
// 			if (tempTCPSenderPtr)
// 			{
// 				tcpSenderPtr.swap(tempTCPSenderPtr);
// 				status = tcpSenderPtr->sendData(tcpSocket, data->getMediaData(), data->getMediaDataSize());
// 			} 
// 			else
// 			{
// 				status = ERR_BAD_ALLOC;
// 			}

			tcpSender.sendData(
				tcpSocket, data.Data(), data.Bytes(), boost::bind(&TCPSession::afterAsyncSendDataNotificationCallback, this, _1, _2));
		}
// 	}
// 
// 	return status;
}

void TCPSession::receivingData(
	boost::system::error_code error, std::size_t transferBytes, const char* transferData /* = nullptr */)
{
	if (!error)
	{
		receiveData();
	}
	else
	{
		//避免重复执行关闭操作
		if (!stopped)
		{
			closeSession();
		}
	}
}

void TCPSession::closingSession(void)
{
	tcpSocket->close();
	boost::checked_delete(tcpSocket);
	stopped = true;
}

NS_END
