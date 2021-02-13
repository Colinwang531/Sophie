#include "boost/bind.hpp"
#include "boost/format.hpp"
#ifdef WINDOWS
#include "glog/log_severity.h"
#include "glog/logging.h"
#else
#include "glog/log_severity.h"
#include "glog/logging.h"
#endif//WINDOWS
#include "Error.h"
#include "Define.h"
#include "Time/XTime.h"
using Time = framework::time::Time;
#include "Thread/ThreadPool.h"
using ThreadPool = framework::thread::ThreadPool;
#include "Network/ZMQ/Msg_.h"
using Msg = framework::network::zeromq::Msg;
#include "CMS.h"

CMS::CMS(
	const std::string name, 
	const std::string id) 
	: OD(), cmsName{ name }, cmsID{ id }, pairSequenceNumber{ 0 }
{}
CMS::~CMS()
{}

int CMS::startOD(
	const char* ipv4 /* = nullptr */,
	const unsigned short port /* = 61001 */)
{
	LOG(INFO) << "Starting CMS with parameters of command line [ name = " << cmsName <<
		", remote XMQ IPv4 = " << ipv4 <<
		", remote XMQ port = " << port << " ].";

	int e{ OD::startOD(ipv4, port) };

	if (eSuccess == e)
	{
		LOG(INFO) << "Starting CMS service successfully.";
		ThreadPool::get_mutable_instance().createNewThread(
			boost::bind(&CMS::sendPairMessageThread, this));
		ThreadPool::get_mutable_instance().createNewThread(
			boost::bind(&CMS::checkRegisterComponentTimeoutThread, this));
	}
	else
	{
		LOG(WARNING) << "Starting CMS service failed, result = " << e << ".";
	}

	return e;
}

int CMS::stopOD()
{
	int e{ OD::stopOD() };

	if (eSuccess == e)
	{
		LOG(INFO) << "Stopping CMS service successfully.";
	}
	else
	{
		LOG(WARNING) << "Stopping CMS service failed, result = " << e << ".";
	}

	return e;
}

void CMS::afterParsePolledMessage(
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

	//处理CMS配对应答消息
	if (!command.compare(gPairCommandName))
	{
		processXMQPairMessage(
			module, from, to, sequence, timestamp, message);
	}
	//处理组件注册请求消息
	else if(!command.compare(gComponentCommandName))
	{ 
		processComponentMessage(
			module, from, to, routers, sequence, timestamp, message);
	}
	//处理设备配置请求消息
	else if (!command.compare(gDeviceCommandName))
	{
		processDeviceMessage(module, from, to, routers, messages);
	}
	//处理算法配置请求消息
	else if (!command.compare(gAlgorithmCommandName))
	{
		processAlgorithmMessage(module, from, to, routers, messages);
	}
	//处理人脸底库配置请求消息
	else if (!command.compare(gCrewCommandName))
	{
		processCrewMessage(module, from, to, routers, messages);
	}
	//处理状态配置请求消息
	else if (!command.compare(gStatusCommandName))
	{
		processStatusMessage(module, from, to, routers, messages);
	}
	//处理设备抓图请求消息
// 	else if (!command.compare(gCaptureCommandName))
// 	{
// 	}
	//处理AIS数据推送消息
// 	else if (!command.compare(gAISCommandName))
// 	{
// 	}
	//处理Clock数据推送消息
	else if (!command.compare(gClockCommandName))
	{
		processClockMessage(module, from, to, routers, messages);
	}
	//处理报警数据推送消息
	else if (!command.compare(gAlarmCommandName))
	{
		processAlarmMessage(module, from, to, routers, messages);
	}
	//处理数据同步推送消息
// 	else if (!command.compare(gUplinkCommandName))
// 	{
// 	}
	else
	{
		LOG(WARNING) << "CMS polled unknown command type of message with from [ " << from <<
			" ] to [ " << to <<
			" ] command [ " << command << " ].";
	}
}

void CMS::sendPairMessageThread()
{
	unsigned long long lastTickcount{ 0 };

	while (!OD::isStopped())
	{
		const unsigned long long currentTickcount{ Time().tickcount() };

		if (30000 < currentTickcount - lastTickcount)
		{
			//-------------------------------------------------------------------------------
			//| "" | "worker" | "cms" | "xmq" | "" | sequence | timestamp | "pair" | "name" |
			//-------------------------------------------------------------------------------
			Msg msg;
			msg.addMessage(cmsName);
			msg.addMessage(gPairCommandName);
			msg.addMessage((boost::format("%d") % (int)Time().tickcount()).str());
			msg.addMessage((boost::format("%d") % ++pairSequenceNumber).str());
			msg.addMessage(gEmptyData);
			msg.addMessage(gXMQComponentName);
			msg.addMessage(gCMSComponentName);
			msg.addMessage(gWorkerModuleName);
			msg.addMessage(gEmptyData);

			int e{ OD::sendMsg(&msg) };
			if (eSuccess == e)
			{
				LOG(INFO) << "CMS send message for pairing successfully.";
			}
			else
			{
				LOG(WARNING) << "CMS send message for pairing failed, result = " << e << ".";
			}

			lastTickcount = currentTickcount;
		}

		Time().sleep(1);
	}
}

void CMS::checkRegisterComponentTimeoutThread()
{
	while (!OD::isStopped())
	{
		std::vector<RegisterComponent> components;
		registerComponentGroup.values(components);

		for (int i = 0; i != components.size(); ++i)
		{
			const unsigned long long expireTickcount{ Time().tickcount() - components[i].timestamp };

			if (90000 < expireTickcount)
			{
				registerComponentGroup.remove(components[i].component.cds[0].id);
				LOG(WARNING) << "Remove component [ " << components[i].component.cds[0].name << 
					" : " << components[i].component.cds[0].ipv4 << 
					" : " << components[i].component.cds[0].type <<
					" ] while expiring " << expireTickcount / 1000 << 
					" seconds.";
			}
		}

		Time().sleep(1);
	}
}

void CMS::processXMQPairMessage(
	const std::string module,
	const std::string from,
	const std::string to,
	const std::string sequence,
	const std::string timestamp,
	const std::string msg)
{
	LOG(INFO) << "CMS process status message of pairing with module [  " << module <<
		" ] from [ " << from <<
		" ] to [ " << to <<
		" ] as status = " << msg << 
		", sequence = " << sequence <<
		", timestamp = " << timestamp << ".";
}

void CMS::processComponentMessage(
	const std::string module, 
	const std::string from, 
	const std::string to, 
	const std::vector<std::string> routers, 
	const std::string sequence, 
	const std::string timestamp, 
	const std::string msg)
{
	RegisterComponent rc;
	int e{ ProtocolComponent().unpack(msg, rc.component) };

	if (eSuccess == e)
	{
		if (1 == rc.component.command)
		{
			//SIGNIN_REQ

			rc.timestamp = Time().tickcount();
			rc.routers = routers;
			//组件注册时0#实例一定存在
			//先删除再添加以示更新
			registerComponentGroup.remove(rc.component.cds[0].id);
			registerComponentGroup.insert(rc.component.cds[0].id, rc);
			rc.component.status = e;
			++rc.component.command;

			LOG(INFO) << "CMS process component register message with module [  " << module <<
				" ] from [ " << from <<
				" ] to [ " << to <<
				" ] ID [ " << rc.component.cds[0].id <<
				" ] Count [ " << registerComponentGroup.size() <<
				" ], sequence = " << sequence <<
				", timestamp = " << timestamp << ".";
		}
		else if (2 == rc.component.command)
		{
			//SIGNIN_REP
			//不能应答
//			parentXMQID = reinterpret_cast<const char*>(pkt->getPacketData());
			return;
		}
		else if (3 == rc.component.command)
		{
			////SIGNOUT_REQ
//			removeComponentByID(ac->getComponentID());
		}
		else if (5 == rc.component.command)
		{
			//QUERY_REQ

			std::vector<RegisterComponent> components;
			registerComponentGroup.values(components);
			for (int i = 0; i != components.size(); ++i)
			{
				//组件注册时0#实例一定存在
				rc.component.cds.push_back(components[i].component.cds[0]);
			}
			++rc.component.command;
			rc.component.status = e;

			LOG(INFO) << "CMS process component query message with module [  " << module <<
				" ] from [ " << from <<
				" ] to [ " << to <<
				" ] Count [ " << rc.component.cds.size() <<
				" ], sequence = " << sequence <<
				", timestamp = " << timestamp << ".";
		}
	}
	else
	{
		LOG(WARNING) << "CMS process component message with module [  " << module <<
			" ] from [ " << from <<
			" ] to [ " << to <<
			" ], sequence = " << sequence <<
			", timestamp = " << timestamp << 
			" failed, result = " << e << ".";
	}
	
	//-------------------------------------------------------------------------------------
	//| "" | "worker" | "cms" | "xmq" | "" | sequence | timestamp | "component" | message |
	//-------------------------------------------------------------------------------------
	Msg msg_;
	msg_.addMessage(ProtocolComponent().pack(rc.component));
	msg_.addMessage(gComponentCommandName);
	msg_.addMessage(timestamp);
	msg_.addMessage(sequence);
	msg_.addMessage(gEmptyData);
	//发送端ID标识
	msg_.addMessage(routers[0]);
	msg_.addMessage(from);
	msg_.addMessage(to);
	msg_.addMessage(module);
	msg_.addMessage(gEmptyData);

	if (eSuccess == OD::sendMsg(&msg_))
	{
		LOG(INFO) << "CMS send message of component successfully.";
	}
	else
	{
		LOG(WARNING) << "CMS send message of component failed, result = " << e << ".";
	}
}

void CMS::processDeviceMessage(
	const std::string module, 
	const std::string from, 
	const std::string to, 
	const std::vector<std::string> routers, 
	const std::vector<std::string> messages)
{
	//查找DHS组件并转发
	std::vector<RegisterComponent> rc;
	registerComponentGroup.values(rc);

	for (int i = 0; i != rc.size(); ++i)
	{
		if (5 == rc[i].component.cds[0].type)
		{
			//--------------------------------------------------------------------------------------------------------
			//| "" | "worker" | "web" | "cms" | DHS CommID | sender | "" | sequence | timestamp | "device" | message |
			//--------------------------------------------------------------------------------------------------------
			Msg msg;
			int msgNum{ static_cast<int>(messages.size()) };
			for (int i = msgNum; i != 0; --i)
			{
				msg.addMessage(messages[i - 1]);
			}
			msg.addMessage(gEmptyData);
			if (0 < routers.size())
			{
				msg.addMessage(routers[0]);
			}
			msg.addMessage(rc[i].routers[0]);
			msg.addMessage(to);
			msg.addMessage(from);
			msg.addMessage(module);
			msg.addMessage(gEmptyData);

			if (eSuccess == OD::sendMsg(&msg))
			{
				LOG(INFO) << "CMS forward message of device successfully.";
			}
			else
			{
				LOG(WARNING) << "CMS forward message of device failed.";
			}
		}
	}
}

void CMS::processClockMessage(
	const std::string module,
	const std::string from,
	const std::string to,
	const std::vector<std::string> routers,
	const std::vector<std::string> messages)
{
	//查找ALM组件并转发
	std::vector<RegisterComponent> rc;
	registerComponentGroup.values(rc);

	for (int i = 0; i != rc.size(); ++i)
	{
		if (17 == rc[i].component.cds[0].type)
		{
			//-------------------------------------------------------------------------------------------------------
			//| "" | "worker" | "web" | "cms" | ALM CommID | sender | "" | sequence | timestamp | "clock" | message |
			//-------------------------------------------------------------------------------------------------------
			Msg msg;
			int msgNum{ static_cast<int>(messages.size()) };
			for (int i = msgNum; i != 0; --i)
			{
				msg.addMessage(messages[i - 1]);
			}
			msg.addMessage(gEmptyData);
			if (0 < routers.size())
			{
				msg.addMessage(routers[0]);
			}
			msg.addMessage(rc[i].routers[0]);
			msg.addMessage(to);
			msg.addMessage(from);
			msg.addMessage(module);
			msg.addMessage(gEmptyData);
			int e{ OD::sendMsg(&msg) };

			if (eSuccess == e)
			{
				LOG(INFO) << "CMS send clock message successfully.";
			}
			else
			{
				LOG(WARNING) << "CMS send clock message failed, result = " << e << ".";
			}
		}
	}
}

void CMS::processAlgorithmMessage(
	const std::string module,
	const std::string from,
	const std::string to,
	const std::vector<std::string> routers,
	const std::vector<std::string> messages)
{
	//查找算法组件并转发
	std::vector<RegisterComponent> rc;
	registerComponentGroup.values(rc);

	for (int i = 0; i != rc.size(); ++i)
	{
		if (18 == rc[i].component.cds[0].type || 
			19 == rc[i].component.cds[0].type || 
			20 == rc[i].component.cds[0].type || 
			21 == rc[i].component.cds[0].type || 
			22 == rc[i].component.cds[0].type)
		{
			//------------------------------------------------------------------------------------------------------------
			//| "" | "worker" | "web" | "cms" | Algo CommID | sender | "" | sequence | timestamp | "algorithm" | message |
			//------------------------------------------------------------------------------------------------------------
			Msg msg;
			int msgNum{ static_cast<int>(messages.size()) };
			for (int i = msgNum; i != 0; --i)
			{
				msg.addMessage(messages[i - 1]);
			}
			msg.addMessage(gEmptyData);
			if (0 < routers.size())
			{
				msg.addMessage(routers[0]);
			}
			msg.addMessage(rc[i].routers[0]);
			msg.addMessage(to);
			msg.addMessage(from);
			msg.addMessage(module);
			msg.addMessage(gEmptyData);
			int e{ OD::sendMsg(&msg) };

			if (eSuccess == e)
			{
				LOG(INFO) << "CMS forward message of algorithm successfully.";
			}
			else
			{
				LOG(WARNING) << "CMS forward message of algorithm failed, result = " << e << ".";
			}
		}
	}
}

void CMS::processStatusMessage(
	const std::string module,
	const std::string from,
	const std::string to,
	const std::vector<std::string> routers,
	const std::vector<std::string> messages)
{
	//查找算法组件并转发
	std::vector<RegisterComponent> rc;
	registerComponentGroup.values(rc);

	for (int i = 0; i != rc.size(); ++i)
	{
		if (18 == rc[i].component.cds[0].type ||
			19 == rc[i].component.cds[0].type ||
			20 == rc[i].component.cds[0].type ||
			21 == rc[i].component.cds[0].type ||
			22 == rc[i].component.cds[0].type)
		{
			//---------------------------------------------------------------------------------------------------------
			//| "" | "worker" | "web" | "cms" | Algo CommID | sender | "" | sequence | timestamp | "status" | message |
			//---------------------------------------------------------------------------------------------------------
			Msg msg;
			int msgNum{ static_cast<int>(messages.size()) };
			for (int i = msgNum; i != 0; --i)
			{
				msg.addMessage(messages[i - 1]);
			}
			msg.addMessage(gEmptyData);
			if (0 < routers.size())
			{
				msg.addMessage(routers[0]);
			}
			msg.addMessage(rc[i].routers[0]);
			msg.addMessage(to);
			msg.addMessage(from);
			msg.addMessage(module);
			msg.addMessage(gEmptyData);
			int e{ OD::sendMsg(&msg) };

			if (eSuccess == e)
			{
				LOG(INFO) << "CMS forward message of status successfully.";
			}
			else
			{
				LOG(WARNING) << "CMS forward message of status failed, result = " << e << ".";
			}
		}
	}
}

void CMS::processCrewMessage(
	const std::string module,
	const std::string from,
	const std::string to,
	const std::vector<std::string> routers,
	const std::vector<std::string> messages)
{
	//查找算法组件并转发
	std::vector<RegisterComponent> rc;
	registerComponentGroup.values(rc);

	for (int i = 0; i != rc.size(); ++i)
	{
		if (21 == rc[i].component.cds[0].type)
		{
			//-------------------------------------------------------------------------------------------------------
			//| "" | "worker" | "web" | "cms" | Algo CommID | sender | "" | sequence | timestamp | "crew" | message |
			//-------------------------------------------------------------------------------------------------------
			Msg msg;
			int msgNum{ static_cast<int>(messages.size()) };
			for (int i = msgNum; i != 0; --i)
			{
				msg.addMessage(messages[i - 1]);
			}
			msg.addMessage(gEmptyData);
			if (0 < routers.size())
			{
				msg.addMessage(routers[0]);
			}
			msg.addMessage(rc[i].routers[0]);
			msg.addMessage(to);
			msg.addMessage(from);
			msg.addMessage(module);
			msg.addMessage(gEmptyData);
			int e{ OD::sendMsg(&msg) };

			if (eSuccess == e)
			{
				LOG(INFO) << "CMS forward message of crew successfully.";
			}
			else
			{
				LOG(WARNING) << "CMS forward message of crew failed, result = " << e << ".";
			}
		}
	}
}

void CMS::processAlarmMessage(
	const std::string module,
	const std::string from,
	const std::string to,
	const std::vector<std::string> routers,
	const std::vector<std::string> messages)
{
	//查找Alarm组件并转发
	std::vector<RegisterComponent> rc;
	registerComponentGroup.values(rc);

	for (int i = 0; i != rc.size(); ++i)
	{
		if (17 == rc[i].component.cds[0].type)
		{
			//--------------------------------------------------------------------------------------------------------
			//| "" | "worker" | "web" | "cms" | DHS CommID | sender | "" | sequence | timestamp | "device" | message |
			//--------------------------------------------------------------------------------------------------------
			Msg msg;
			int msgNum{ static_cast<int>(messages.size()) };
			for (int i = msgNum; i != 0; --i)
			{
				msg.addMessage(messages[i - 1]);
			}
			msg.addMessage(gEmptyData);
			if (0 < routers.size())
			{
				msg.addMessage(routers[0]);
			}
			msg.addMessage(rc[i].routers[0]);
			msg.addMessage(to);
			msg.addMessage(from);
			msg.addMessage(module);
			msg.addMessage(gEmptyData);

			if (eSuccess == OD::sendMsg(&msg))
			{
				LOG(INFO) << "CMS forward message of alarm successfully.";
			}
			else
			{
				LOG(WARNING) << "CMS forward message of alarm failed.";
			}
		}
	}
}
