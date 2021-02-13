#include "boost/bind.hpp"
#ifdef _WINDOWS
#include "glog/log_severity.h"
#include "glog/logging.h"
#else
#include "glog/log_severity.h"
#include "glog/logging.h"
#endif//_WINDOWS
#include "Define.h"
#include "Error.h"
#include "DAS.h"

DAS::DAS() : asio(), so{ nullptr }
{}
DAS::~DAS()
{}

int DAS::startDAS(const unsigned short recvPort/* = 0*/)
{
	int e{ 
		gMinPortNumber < recvPort && gMaxPortNumber > recvPort ? asio.startASIO() : eInvalidParameter };

	if (eSuccess == e)
	{
		LOG(INFO) << "DAS service started successfully. ";
		void* s{ 
			asio.createNewSocket(framework::network::asio::SocketType::SOCKET_UDP, recvPort) };

		if (s && eSuccess == UDPSession::receiveData(s))
		{
			so = s;
		}
		else
		{
			asio.destroySocket(framework::network::asio::SocketType::SOCKET_UDP, s);
			LOG(WARNING) << "DAS service created new socket and session failed.";
		}

	}
	else
	{
		LOG(WARNING) << "DAS service started failed, result = " << e << ".";
	}

	return e;
}

int DAS::stopDAS()
{
	return asio.stopASIO();
}

void DAS::afterReceivedDataNotification(
	const int e /* = 0 */,
	DataPtr data /* = nullptr */,
	const char* ipv4 /* = nullptr */,
	const unsigned short port /* = 0 */)
{
	if (!e && data && ipv4 && 0 < port)
	{
		const char* recvdata{ 
			reinterpret_cast<const char*>(data->getData()) };
		const size_t addressNameBytes{ strlen(gAddressCommandName) };

		if (!strncmp(recvdata, gAddressCommandName, addressNameBytes))
		{
			if (eSuccess == data->setData(ipv4, strlen(ipv4)))
			{
				UDPSession::sendData(so, data, ipv4, port + 1);
			}
		}
		
		LOG(INFO) << "DAS read data from [ " << ipv4 << " : " << port << " ], data = " << recvdata << ".";
	}
	else
	{
		LOG(WARNING) << "DAS read data from [ " << ipv4 << " : " << port << " ], data = " << data << ".";
	}
}

// void DAS::afterCatchExceptionNotification(const int e /* = 0 */)
// {}
// 
// void DAS::afterCatchTimeoutdNotification()
// {}
