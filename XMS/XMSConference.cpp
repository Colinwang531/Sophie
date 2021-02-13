#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "Error.h"
#include "Time/XTime.h"
using Time = framework::time::Time;
#include "Data/Buffer.h"
#include "Network/ASIO/Session/TCPSourceSession.h"
using TCPSourceSession = framework::network::asio::TCPSourceSession;
using TCPSourceSessionPtr = boost::shared_ptr<TCPSourceSession>;
#include "XMSConference.h"

XMSConference::XMSConference()
{}
XMSConference::~XMSConference()
{}

int XMSConference::addSource(
	boost::asio::ip::tcp::socket& so, 
	const std::string url,
	const char* ipv4 /* = nullptr */)
{
	int e{ !url.empty() && ipv4 ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		//XMS的端口号默认为60927
		//这个需要在后期将端口配置到URL中
		TCPSessionPtr tsp{
			boost::make_shared<TCPSourceSession>(
				url,
				so,
				boost::bind(&XMSConference::afterTCPSendResultCallback, this, _1, _2),
				boost::bind(&XMSConference::afterTCPReceiveResultCallback, this, _1, _2, _3)) };
		if (tsp)
		{
			TCPSourceSessionPtr tssp{
				boost::dynamic_pointer_cast<TCPSourceSession>(tsp) };
			e = tssp->connect(ipv4, 60927);

			if (eSuccess == e)
			{
				source.swap(tsp);
			}
		}
		else
		{
			e = eBadNewObject;
		}
	}

	return e;
}

int XMSConference::addTarget(TCPSessionPtr tsp)
{
	int e{ tsp ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		targets.pushBack(tsp);
	}

	return e;
}

void XMSConference::afterTCPSendResultCallback(
	const int e /* = 0 */, 
	const int bytes /* = 0 */)
{

}

void XMSConference::afterTCPReceiveResultCallback(
	const int e /* = 0 */,
	const void* data /* = nullptr */,
	const int bytes /* = 0 */)
{
	if (!e && data && 0 < bytes)
	{
		for (int i = 0; i != targets.size(); ++i)
		{
			targets.at(i)->send(data, bytes);
		}
	}
}
