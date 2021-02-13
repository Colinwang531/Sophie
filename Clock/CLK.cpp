#include "boost/algorithm/string.hpp"
#include "boost/bind.hpp"
#include "boost/format.hpp"
#ifdef _WINDOWS
#include "glog/log_severity.h"
#include "glog/logging.h"
#else
#include "glog/log_severity.h"
#include "glog/logging.h"
#endif//_WINDOWS
#include "Error.h"
#include "Define.h"
#include "Thread/ThreadPool.h"
using ThreadPool = framework::thread::ThreadPool;
#include "Time/XTime.h"
using Time = framework::time::Time;
#include "Data/Protocol/ProtocolComponent.h"
using ProtocolComponent = framework::data::ProtocolComponent;
#include "Data/Protocol/ProtocolClock.h"
using ProtocolClock = framework::data::ProtocolClock;
#include "Network/ZMQ/Msg_.h"
using Msg = framework::network::zeromq::Msg;
#include "CLK.h"

CLK::CLK(
	const std::string name,
	const std::string id,
	const int number /* = -1 */,
	const int br /* = 4800 */)
	: XMQPeer(name, id), Com(), comNumber{ number }, baudRate{ br }, clockSequenceNumber{ 0 }
{}
CLK::~CLK() {}

int CLK::startOD(
	const char* ipv4 /* = nullptr */, 
	const unsigned short port /* = 0 */)
{
	int e{ XMQPeer::startOD(ipv4, port) };

	if (eSuccess == e)
	{
		LOG(INFO) << "Starting CLK with parameters of command line [ name = " << XMQPeer::getName() <<
			", remote XMQ IPv4 = " << ipv4 <<
			", remote XMQ port = " << port << " ].";

		e = Com::openCom(comNumber, baudRate);

		if (eSuccess == e)
		{
			LOG(INFO) << "CLK open com [ " << comNumber << " ] with baudrate [ " << baudRate << " ] successfully.";
		}
		else
		{
			LOG(WARNING) << "CLK open com [ " << comNumber << " ] with baudrate [ " << baudRate << " ] failed, result = " << e << ".";
		}
	}
	else
	{
		LOG(WARNING) << "Starting AIS with parameters of command line [ name = " << XMQPeer::getName() <<
			", remote XMQ IPv4 = " << ipv4 <<
			", remote XMQ port = " << port <<
			" ] failed, result = " << e << ".";
	}

	return e;
}

int CLK::stopOD()
{
	int e{ XMQPeer::stopOD() };

	if (eSuccess == e)
	{
		LOG(INFO) << "Stopping CLK service successfully.";

		e = Com::closeCom();

		if (eSuccess == e)
		{
			LOG(INFO) << "CLK close com [ " << comNumber << " ] with baudrate [ " << baudRate << " ] successfully.";
		}
		else
		{
			LOG(WARNING) << "CLK close com [ " << comNumber << " ] with baudrate [ " << baudRate << " ] failed, result = " << e << ".";
		}
	}
	else
	{
		LOG(WARNING) << "Stopping CLK service failed, result = " << e << ".";
	}

	return e;
}

void CLK::afterParsePolledMessage(
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
	const std::string message{ messages[msgDataNumber - 1] };

	//处理组件注册请求消息
	if (!command.compare(gComponentCommandName))
	{
		processComponentMessage(module, from, to, sequence, timestamp, message);
	}
}

void CLK::afterReceiveComDataNotification(
	const char* data /* = nullptr */, 
	const int bytes /* = 0 */)
{
	if (data && 0 < bytes)
	{
		comData.append(data, bytes);

		if ('\n' == *data)
		{
			if (!comData.empty())
			{
				std::vector<std::string> clockDatas;
				boost::split(clockDatas, comData, boost::is_any_of(","));

				if (!clockDatas[0].compare("$ZQZDA") || !clockDatas[0].compare("$CJZDA") || !clockDatas[0].compare("$GPZDA"))
				{
					char clockDesc[128]{ 0 };
#ifdef WINDOWS
					sprintf_s(clockDesc, 128, "%s-%s-%s,%s:%s:%s,%s",
						clockDatas[4].c_str(), clockDatas[3].c_str(), clockDatas[2].c_str(),
						clockDatas[1].substr(0, 2).c_str(), clockDatas[1].substr(2, 2).c_str(), clockDatas[1].substr(4, 2).c_str(),
						clockDatas[5].c_str());
#else
					sprintf(clockDesc, "%s-%s-%s,%s:%s:%s,%s",
						clockDatas[4].c_str(), clockDatas[3].c_str(), clockDatas[2].c_str(),
						clockDatas[1].substr(0, 2).c_str(), clockDatas[1].substr(2, 2).c_str(), clockDatas[1].substr(4, 2).c_str(),
						clockDatas[5].c_str());
#endif//WINDOWS
					
					//发送数据
					sendClockMessage(clockDesc);
				}

				comData.clear();
			}
		}
	} 
	else
	{
		LOG(WARNING) << "Receive COM data failed.";
	}
}

const std::string CLK::buildRegisterMessage()
{
	using ComponentMsg = framework::data::ComponentMsg;
	ComponentMsg c;
	c.command = 1;
	c.status = -1;
	using ComponentData = framework::data::ComponentData;
	ComponentData cd;
	cd.type = 16;
	cd.name = XMQPeer::getName();
	cd.nickname = XMQPeer::getName();
	cd.id = XMQPeer::getID();
	c.cds.push_back(cd);

	return ProtocolComponent().pack(c);
}

void CLK::processComponentMessage(
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

			LOG(INFO) << "Clock process component message with module [  " << module <<
				" ] from [ " << from <<
				" ] to [ " << to <<
				" ], sequence = " << sequence <<
				", timestamp = " << timestamp <<
				", status = " << cm.status << ".";
		}
	}
	else
	{
		LOG(WARNING) << "Clock process component message with module [  " << module <<
			" ] from [ " << from <<
			" ] to [ " << to <<
			" ], sequence = " << sequence <<
			", timestamp = " << timestamp <<
			" failed, result = " << e << ".";
	}
}

int CLK::sendClockMessage(const std::string clock)
{
	//---------------------------------------------------------------------------------
	//| "" | "worker" | "clk" | "cms" | "" | sequence | timestamp | "clock" | message |
	//---------------------------------------------------------------------------------
	Msg msg;
	msg.addMessage(ProtocolClock().pack(clock));
	msg.addMessage(gClockCommandName);
	msg.addMessage((boost::format("%d") % (int)Time().tickcount()).str());
	msg.addMessage((boost::format("%d") % ++clockSequenceNumber).str());
	msg.addMessage(gEmptyData);
	msg.addMessage(gCMSComponentName);
	msg.addMessage(gCLKComponentName);
	msg.addMessage(gWorkerModuleName);
	msg.addMessage(gEmptyData);

	int e{ OD::sendMsg(&msg) };
	if (eSuccess == e)
	{
		//间隔30m打印一次
		if (0 == clockSequenceNumber % 1800)
		{
			LOG(INFO) << "Clock send clock data [ " << clock << " ].";
		}
	}
	else
	{
		LOG(WARNING) << "Clock send clock data [ " << clock << " ] failed, result = " << e << ".";
	}

	return e;
}
