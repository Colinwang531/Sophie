#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
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
#include "Time/XTime.h"
using Time = framework::time::Time;
#include "Data/Protocol/ProtocolComponent.h"
using ProtocolComponent = framework::data::ProtocolComponent;
#include "Data/Protocol/ProtocolAIS.h"
using ProtocolAIS = framework::data::ProtocolAIS;
#include "Network/ZMQ/Msg_.h"
using Msg = framework::network::zeromq::Msg;
#include "Data/AIS/AISData.h"
using AISData = framework::data::AISData;
#include "AIS.h"

AIS::AIS(
	const std::string name,
	const std::string id,
	const int number /* = -1 */,
	const int br /* = 38400 */)
	: XMQPeer(name, id), Com(), comNumber{ number }, baudRate{ br }
{}
AIS::~AIS() {}

int AIS::startOD(
	const char* ipv4 /* = nullptr */, 
	const unsigned short port /* = 0 */)
{
	int e{ OD::startOD(ipv4, port) };

	if (eSuccess == e)
	{
		LOG(INFO) << "Starting AIS with parameters of command line [ name = " << XMQPeer::getName() <<
			", remote XMQ IPv4 = " << ipv4 <<
			", remote XMQ port = " << port << " ].";

		e = Com::openCom(comNumber, baudRate);

		if (eSuccess == e)
		{
			LOG(INFO) << "AIS open com [ " << comNumber << " ] with baudrate [ " << baudRate << " ] successfully.";
		}
		else
		{
			LOG(WARNING) << "AIS open com [ " << comNumber << " ] with baudrate [ " << baudRate << " ] failed, result = " << e << ".";
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

int AIS::stopOD()
{
	int e{ OD::stopOD() };

	if (eSuccess == e)
	{
		LOG(INFO) << "Stopping AIS service successfully.";

		e = Com::closeCom();

		if (eSuccess == e)
		{
			LOG(INFO) << "AIS close com [ " << comNumber << " ] with baudrate [ " << baudRate << " ] successfully.";
		}
		else
		{
			LOG(WARNING) << "AIS close com [ " << comNumber << " ] with baudrate [ " << baudRate << " ] failed, result = " << e << ".";
		}
	}
	else
	{
		LOG(WARNING) << "Stopping AIS service failed, result = " << e << ".";
	}

	return e;
}

void AIS::afterParsePolledMessage(
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

void AIS::afterReceiveComDataNotification(
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
				std::vector<std::string> aisDatas;
				boost::split(aisDatas, comData, boost::is_any_of(","));
				const std::string flags{ aisDatas[0] };

				if ('$' == flags[0] || 'V' == flags[3] || 'D' == flags[4] || 'O' == flags[5])
				{
					const int totalNumber{ atoi(aisDatas[1].c_str()) };
					const int currentNumber{ atoi(aisDatas[2].c_str()) };

					if (totalNumber > currentNumber)
					{
						parseData.append(aisDatas[5]);
					}
					else if (totalNumber == currentNumber)
					{
						parseData.append(aisDatas[5]);

						DataPtr dp{ 
							boost::make_shared<Data>(
								framework::data::DataFactory::DATA_FACTORY_AIS, 
								framework::data::DataType::DATA_TYPE_BYTES) };

						if (dp)
						{
							dp->setData(parseData.c_str(), parseData.length());
							Decoder::inputData(dp);
						}
					}
				}
			}

			comData.clear();
		}
	} 
	else
	{
		LOG(WARNING) << "Receive COM data failed.";
	}
}

const std::string AIS::buildRegisterMessage()
{
	using ComponentMsg = framework::data::ComponentMsg;
	ComponentMsg c;
	c.command = 1;
	c.status = -1;
	using ComponentData = framework::data::ComponentData;
	ComponentData cd;
	cd.type = 15;
	cd.name = XMQPeer::getName();
	cd.nickname = XMQPeer::getName();
	cd.id = XMQPeer::getID();
	c.cds.push_back(cd);

	return ProtocolComponent().pack(c);
}

void AIS::afterDataDecodeNotification(DataPtr data)
{
	boost::shared_ptr<AISData> dp{ 
		boost::dynamic_pointer_cast<AISData>(data) };

	if (dp)
	{
		AISMsg msg;
		msg.type = dp->msgID;

		if (1 == msg.type || 2 == msg.type || 3 == msg.type)
		{
			msg.positiona = dp->pa;
		}
		else if (18 == msg.type)
		{
			msg.positionb = dp->pb;
		}
		else if (5 == msg.type)
		{
			msg.shipstatic = dp->ss;
		}
		else if (9 == msg.type)
		{
			msg.standardsar = dp->sar;
		}
		else if (21 == msg.type)
		{
			msg.aidstonavigation = dp->nav;
		}

		sendAISMessage(msg);
	}
}

void AIS::processComponentMessage(
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

			LOG(INFO) << "AIS process component message with module [  " << module <<
				" ] from [ " << from <<
				" ] to [ " << to <<
				" ], sequence = " << sequence <<
				", timestamp = " << timestamp <<
				", status = " << cm.status << ".";
		}
	}
	else
	{
		LOG(WARNING) << "AIS process component message with module [  " << module <<
			" ] from [ " << from <<
			" ] to [ " << to <<
			" ], sequence = " << sequence <<
			", timestamp = " << timestamp <<
			" failed, result = " << e << ".";
	}
}

int AIS::sendAISMessage(const AISMsg& ais)
{
	//-------------------------------------------------------------------------------
	//| "" | "worker" | "clk" | "cms" | "" | sequence | timestamp | "ais" | message |
	//-------------------------------------------------------------------------------
	Msg msg;
	msg.addMessage(ProtocolAIS().pack(ais));
	msg.addMessage(gAISCommandName);
	msg.addMessage((boost::format("%d") % (int)Time().tickcount()).str());
	msg.addMessage((boost::format("%d") % ++aisSequenceNumber).str());
	msg.addMessage(gEmptyData);
	msg.addMessage(gCMSComponentName);
	msg.addMessage(gAISComponentName);
	msg.addMessage(gWorkerModuleName);
	msg.addMessage(gEmptyData);

	int e{ OD::sendMsg(&msg) };
	if (eSuccess == e)
	{
		LOG(INFO) << "AIS send clock data [ " << ais.type << " ].";
	}
	else
	{
		LOG(WARNING) << "AIS send clock data [ " << ais.type << " ] failed, result = " << e << ".";
	}

	return e;
}
