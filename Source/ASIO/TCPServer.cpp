#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "Hardware/Cpu.h"
using CPU = NS(hardware, 1)::Cpu;
#include "ASIO/TCPListener.h"
#include "ASIO/TCPServer.h"

NS_BEGIN(asio, 2)

TCPServer::TCPServer()
{}

TCPServer::~TCPServer()
{}

Int32 TCPServer::startServer(const UInt16 port /* = 60531 */)
{
	Int32 status{ ERR_INVALID_PARAM };

	if (!asioServicePtr && gMinPortNumber < port && gMaxPortNumber > port)
	{
		asioServicePtr.swap(boost::make_shared<ASIOService>());
		if (asioServicePtr)
		{
			const UInt8 tempCPUCoreNumber{ static_cast<UInt8>(CPU().getCPUCoreNumber()) };
			status = asioServicePtr->startService(tempCPUCoreNumber);

			if (ERR_OK == status)
			{
				for (int i = 0; i != tempCPUCoreNumber; ++i)
				{
					boost::shared_ptr<TCPListener> tempTCPListenerPtr{
						boost::make_shared<TCPListener>(port, boost::bind(&TCPServer::afterRemoteConnectedNotificationCallback, this, _1, _2)) };
					if (tempTCPListenerPtr)
					{
						tempTCPListenerPtr->startListen(asioServicePtr->getIdle());
					}
				}
			}
		}
	}

	return status;
}

Int32 TCPServer::stopServer(void)
{
	Int32 status{ ERR_BAD_OPERATE };

	if (asioServicePtr)
	{
		status = asioServicePtr->stopService();
	}

	return status;
}

void TCPServer::afterRemoteConnectedNotificationCallback(boost::asio::ip::tcp::socket* so /* = nullptr */, const Int32 error /* = 0 */)
{
	acceptingRemote(so, error);
}

NS_END
