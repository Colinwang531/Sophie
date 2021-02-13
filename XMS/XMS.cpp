#include "boost/bind.hpp"
#include "boost/pointer_cast.hpp"
#ifdef _WINDOWS
#include "glog/log_severity.h"
#include "glog/logging.h"
#else
#include "glog/log_severity.h"
#include "glog/logging.h"
#endif//_WINDOWS
#include "Error.h"
#include "Define.h"
#include "Hardware/Cpu.h"
using Cpu = framework::hardware::Cpu;
#include "Thread/ThreadPool.h"
using ThreadPool = framework::thread::ThreadPool;
#include "Data/Protocol/ProtocolComponent.h"
using ProtocolComponent = framework::data::ProtocolComponent;
#include "Network/ZMQ/Msg_.h"
using Msg = framework::network::zeromq::Msg;
#include "Network/ASIO/Core/TCPListener.h"
using TCPListener = framework::network::asio::TCPListener;
using TCPListenerPtr = boost::shared_ptr<TCPListener>;
#include "Network/ASIO/Session/TCPTargetSession.h"
using TCPSession = framework::network::asio::TCPSession;
using TCPSessionPtr = boost::shared_ptr<TCPSession>;
using TCPTargetSession = framework::network::asio::TCPTargetSession;
#include "Data/Buffer.h"
#include "XMS.h"

XMS::XMS(
	const std::string name,
	const std::string id,
	const unsigned short port /* = 0 */)
	: XMQPeer(name, id), mediaPort{ port }
{}
XMS::~XMS() 
{}

int XMS::startOD(
	const char* ipv4 /* = nullptr */,
	const unsigned short port /* = 61001 */)
{
	//开启TCP监听
	const int coreNumber{ Cpu().getCPUCoreNumber() };
	for (int i = 0; i != 1/*coreNumber*/; ++i)
	{
		TCPListenerPtr lp{
			boost::make_shared<TCPListener>(
				ASIO::getIdleIO(),
				mediaPort,
				boost::bind(&XMS::afterTCPAcceptResultCallback, this, _1, _2)) };
		if (lp)
		{
			lp->listen();
		}
	}

	return XMQPeer::startOD(ipv4, port);
}

int XMS::stopOD()
{
	int e{ XMQPeer::stopOD() };

	if (eSuccess == e)
	{
		LOG(INFO) << "Stop XMS service successfully.";
	}
	else
	{
		LOG(WARNING) << "Stop XMS service failed, result = " << e << ".";
	}

	return e;
}

void XMS::afterParsePolledMessage(
	const std::string module,
	const std::string from,
	const std::string to,
	const std::vector<std::string> routers,
	const std::vector<std::string> messages)
{
	//消息内容的最后2个部分一定是command和message
	const std::size_t msgDataNumber{ messages.size() };
	const std::string sequence{ messages[msgDataNumber - 4] };
	const std::string timestamp{ messages[msgDataNumber - 3] };
	const std::string command{ messages[msgDataNumber - 2] };
	const std::string msg{ messages[msgDataNumber - 1] };

	//处理组件注册请求消息
	if (!command.compare(gComponentCommandName))
	{
		processComponentMessage(module, from, to, sequence, timestamp, msg);
	}
	else
	{
		LOG(WARNING) << "DHS polled unknown command type of message with from [ " << from <<
			" ] to [ " << to <<
			" ] command [ " << command << " ].";
	}
}

const std::string XMS::buildRegisterMessage()
{
	using ComponentMsg = framework::data::ComponentMsg;
	ComponentMsg c;
	c.command = 1;
	c.status = -1;
	using ComponentData = framework::data::ComponentData;
	ComponentData cd;
	cd.type = 7;
	cd.name = XMQPeer::getName();
	cd.nickname = XMQPeer::getName();
	cd.ipv4 = XMQPeer::getLocalIPv4();
	cd.id = XMQPeer::getID();
	c.cds.push_back(cd);

	return ProtocolComponent().pack(c);
}

void XMS::processComponentMessage(
	const std::string module,
	const std::string from,
	const std::string to,
	const std::string sequence,
	const std::string timestamp,
	const std::string msg)
{
	using ComponentMsg = framework::data::ComponentMsg;
	ComponentMsg cm;
	int e{ ProtocolComponent().unpack(msg, cm) };

	if (eSuccess == e)
	{
		if (2 == cm.command)
		{
			//SIGNIN_REP

			LOG(INFO) << "XMS process component message with module [  " << module <<
				" ] from [ " << from <<
				" ] to [ " << to <<
				" ], sequence = " << sequence <<
				", timestamp = " << timestamp <<
				", status = " << cm.status << ".";
		}
	}
	else
	{
		LOG(WARNING) << "XMS process component message with module [  " << module <<
			" ] from [ " << from <<
			" ] to [ " << to <<
			" ], sequence = " << sequence <<
			", timestamp = " << timestamp <<
			" failed, result = " << e << ".";
	}
}

void XMS::afterTCPAcceptResultCallback(
	const int e, 
	boost::asio::ip::tcp::socket& so)
{
	if (!e)
	{
		TCPSessionPtr tp{
			boost::make_shared<TCPTargetSession>(
				so, 
				boost::bind(&XMS::afterTCPTargetReceiveResultCallback, this, _1, _2, _3)) };

		if (tp)
		{
			tp->receive();
		}
	}
}

void XMS::afterTCPTargetReceiveResultCallback(
	const char* data /* = nullptr */, 
	const int bytes /* = 0 */, 
	TCPSessionPtr tsp /* = nullptr */)
{
	const std::string url{ data, bytes };
	const size_t pos{ url.find_first_of(':') };
	const std::string dhsIP{ url.substr(0, pos) };
	const std::string streamurl{ url.substr(pos + 1, url.length()) };
	XMSConferencePtr xmscp{ conferences.at(streamurl) };

	//创建流源端
	if (!xmscp)
	{
		xmscp = boost::make_shared<XMSConference>();
		if (xmscp)
		{
			conferences.insert(streamurl, xmscp);
			boost::asio::ip::tcp::socket so{ ASIO::getTCPSocket() };
			int e{ xmscp->addSource(so, streamurl, dhsIP.c_str()) };

			if (!e)
			{
				LOG(INFO) << "Add new stream source [ " << streamurl << " ].";
			}
			else
			{
				LOG(WARNING) << "Add new stream source [ " << streamurl << " ] failed, result = " << e << ".";
			}
		}
	}

	//添加流目的端
	int e{ xmscp->addTarget(tsp) };
	if (!e)
	{
		LOG(INFO) << "Add new stream target [ " << streamurl << " ].";
	}
	else
	{
		LOG(WARNING) << "Add new stream target [ " << streamurl << " ] failed, result = " << e << ".";
	}
}
