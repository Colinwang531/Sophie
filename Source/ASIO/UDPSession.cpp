#include "boost/numeric/conversion/cast.hpp"

#include "Errors.h"
#include "Network/UDPSession.h"

namespace network_1
{
	UDPSession::UDPSession(const UDPSocket* so /* = NULL */) : boost::enable_shared_from_this<UDPSession>(), udpSocket(const_cast<UDPSocket*>(so))
	{}

	UDPSession::~UDPSession()
	{}

	void UDPSession::stopSession()
	{
		if (udpSocket)
		{
			udpSocket->close();
			boost::checked_delete<UDPSocket>(udpSocket);
			udpSocket = NULL;
		}
	}

	int UDPSession::bindReadPort(const boost::uint16_t portNum /*= gInvalidPort*/)
	{
		if (gMinPort > portNum || gMaxPort < portNum)
		{
			return gErrors_InvalidParameter;
		}

		if (!udpSocket)
		{
			return gErrors_InitFailed;
		}

		UdpEndpoint localep(boost::asio::ip::udp::v4(), portNum);
		udpSocket->open(boost::asio::ip::udp::v4());
		udpSocket->bind(localep);
		return gErrors_Success;
	}

	int UDPSession::asyncRead()
	{
		if (!udpSocket)
		{
			return Errors_InvalidSocket;
		}

		UDPEndpointPtr senderPtr = boost::make_shared<UdpEndpoint>();
		StreamBufPtr streamBufPtr = boost::make_shared<StreamBuf>(gMTU);
		udpSocket->async_receive_from(
			streamBufPtr->prepare(gMTU), *senderPtr,
			boost::bind(&UDPSession::asyncReadEventNotify, boost::enable_shared_from_this<UDPSession>::shared_from_this(), 
				senderPtr, streamBufPtr, boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));

		return gErrors_Success;
	}

	int UDPSession::asyncReadEventNotify(UDPEndpointPtr udpSenderPtr, StreamBufPtr streamBufPtr, const std::size_t streamBufSize /* = 0 */, const ErrorCode& errorCode /* = ErrorCode() */)
	{
		if (0 < errorCode)
		{
			return Errors_InvalidSocket;
		}

		return asyncRead();
	}

	int UDPSession::asyncWrite(const char* address /* = NULL */, const boost::uint16_t portNum /* = gInvalidPort */, const char* streamBuf /* = NULL */, const boost::uint32_t streamBufSize /* = 0 */)
	{
		if (!address || gMinPort > portNum || gMaxPort < portNum || !streamBuf || 0 == streamBufSize)
		{
			return gErrors_InvalidParameter;
		}

		if (!udpSocket)
		{
			return Errors_InvalidSocket;
		}

		UdpEndpoint targetEndpoint(boost::asio::ip::address::from_string(address), portNum);
		boost::uint32_t nextPacketSize = streamBufSize, currentPos = 0;
		while (currentPos < streamBufSize)
		{
			boost::uint32_t currentPacketSize = (nextPacketSize >= gMTU ? gMTU : nextPacketSize);

			StreamBufPtr streamBufPtr = boost::make_shared<StreamBuf>(currentPacketSize);
			streamBufPtr->prepare(currentPacketSize);
			streamBufPtr->sputn(streamBuf + currentPos, boost::numeric_cast<std::streamsize>(currentPacketSize));
			udpSocket->async_send_to(
				streamBufPtr->data(), targetEndpoint,
				boost::bind(&UDPSession::asyncWriteEventNotify, boost::enable_shared_from_this<UDPSession>::shared_from_this(), 
					streamBufPtr, boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));

			currentPos += currentPacketSize;
			nextPacketSize -= currentPacketSize;
		}

		return gErrors_Success;
	}

	int UDPSession::asyncWriteEventNotify(StreamBufPtr streamBufPtr, const std::size_t streamBufSize /* = 0 */, const ErrorCode& errorCode /* = ErrorCode() */)
	{
		if (0 < errorCode)
		{
			return Errors_InvalidSocket;
		}

		streamBufPtr->consume(streamBufSize);
		return gErrors_Success;
	}
}//namespace network_1
