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
#include "Thread/ThreadPool.h"
using ThreadPool = framework::thread::ThreadPool;
#include "Data/Protocol/ProtocolComponent.h"
using ProtocolComponent = framework::data::ProtocolComponent;
#include "Data/Protocol/ProtocolAlgorithm.h"
using ProtocolAlgorithm = framework::data::ProtocolAlgorithm;
#include "Data/Protocol/ProtocolAlarm.h"
using ProtocolAlarm = framework::data::ProtocolAlarm;
#include "Data/Protocol/ProtocolStatus.h"
using ProtocolStatus = framework::data::ProtocolStatus;
#include "Network/ZMQ/Msg_.h"
using Msg = framework::network::zeromq::Msg;
#include "Graph/CV/LivestreamCVGraph.h"
using LivestreamCVGraph = framework::multimedia::LivestreamCVGraph;
#include "Filter/CV/CVComputingFilter.h"
using FilterPtr = framework::multimedia::FilterPtr;
using CVComputingFilter = framework::multimedia::CVComputingFilter;
// #include "Filter/Network/TCPDataReceiverFilter.h"
// using TCPDataReceiverFilter = framework::multimedia::TCPDataReceiverFilter;
#include "Filter/SDK/HKNetSDKFilter.h"
using HKNetSDKFilter = framework::multimedia::HKNetSDKFilter;
using HKNetSDKFilterPtr = boost::shared_ptr<HKNetSDKFilter>;
#include "Data/Frame/Frame.h"
using Frame = framework::data::Frame;
using FramePtr = boost::shared_ptr<Frame>;
#include "Time/XTime.h"
using Time = framework::time::Time;
#include "Phone.h"

Phone::Phone(
	const std::string name,
	const std::string id)
	: XMQPeer(name, id), workState{ true }, alarmSequenceNumber{ 0 }
{}
Phone::~Phone()
{}

int Phone::startOD(
	const char* ipv4 /* = nullptr */, 
	const unsigned short port /* = 0 */)
{
	int e{ XMQPeer::startOD(ipv4, port) };

	if (eSuccess == e)
	{
		LOG(INFO) << "Start phone with parameters of command line [ name = " << getName() <<
			", remote XMQ IPv4 = " << ipv4 <<
			", remote XMQ port = " << port << " ].";
	}
	else
	{
		LOG(WARNING) << "Start phone with parameters of command line [ name = " << getName() <<
			", remote XMQ IPv4 = " << ipv4 <<
			", remote XMQ port = " << port << " ].";
	}

	return e;
}

int Phone::stopOD()
{
	int e{ XMQPeer::stopOD() };

	if (eSuccess == e)
	{
		LOG(INFO) << "Stop phone service successfully.";
	}
	else
	{
		LOG(WARNING) << "Stop phone service failed, result = " << e << ".";
	}

	return e;
}

void Phone::afterParsePolledMessage(
	const std::string module,
	const std::string from,
	const std::string to,
	const std::vector<std::string> routers,
	const std::vector<std::string> messages)
{
	//��Ϣ���ݵ����2������һ����command��message
	const std::size_t msgDataNumber{ messages.size() };
	const std::string sequence{ messages[msgDataNumber - 4] };
	const std::string timestamp{ messages[msgDataNumber - 3] };
	const std::string command{ messages[msgDataNumber - 2] };
	const std::string msg{ messages[msgDataNumber - 1] };

	//�������ע��������Ϣ
	if (!command.compare(gComponentCommandName))
	{
		processComponentMessage(module, from, to, sequence, timestamp, msg);
	}
	//�����㷨����������Ϣ
	else if (!command.compare(gAlgorithmCommandName))
	{
		processAlgorithmMessage(module, from, to, routers, sequence, timestamp, msg);
	}
	//��������״̬������Ϣ
	else if (!command.compare(gStatusCommandName))
	{
		processStatusMessage(module, from, to, routers, sequence, timestamp, msg);
	}
	else
	{
		LOG(WARNING) << "Phone polled unknown command type of message with from [ " << from <<
			" ] to [ " << to <<
			" ] command [ " << command << " ].";
	}
}

const std::string Phone::buildRegisterMessage()
{
	using ComponentMsg = framework::data::ComponentMsg;
	ComponentMsg c;
	c.command = 1;
	c.status = -1;
	using ComponentData = framework::data::ComponentData;
	ComponentData cd;
	cd.type = 19;
	cd.name = XMQPeer::getName();
	cd.nickname = XMQPeer::getName();
	cd.ipv4 = XMQPeer::getLocalIPv4();
	cd.id = XMQPeer::getID();
	c.cds.push_back(cd);

	return ProtocolComponent().pack(c);
}

void Phone::processComponentMessage(
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

			LOG(INFO) << "Phone process component message with module [  " << module <<
				" ] from [ " << from <<
				" ] to [ " << to <<
				" ], sequence = " << sequence <<
				", timestamp = " << timestamp <<
				", status = " << cm.status << ".";
		}
	}
	else
	{
		LOG(WARNING) << "Phone process component message with module [  " << module <<
			" ] from [ " << from <<
			" ] to [ " << to <<
			" ], sequence = " << sequence <<
			", timestamp = " << timestamp <<
			" failed, result = " << e << ".";
	}
}

void Phone::processAlgorithmMessage(
	const std::string module,
	const std::string from,
	const std::string to,
	const std::vector<std::string> routers,
	const std::string sequence,
	const std::string timestamp,
	const std::string msg)
{
	using AlgorithmMsg = framework::data::AlgorithmMsg;
	AlgorithmMsg am;
	int e{ ProtocolAlgorithm().unpack(msg, am) };

	//�ж��Ƿ��ǰ�ȫñ�㷨
	//���������κδ����Ӧ��
	if (2 != am.algos[0].type)
	{
		return;
	}

	if (eSuccess == e)
	{
		if (1 == am.command)
		{
			//NEW_REQ
			GraphPtr graph{ boost::make_shared<LivestreamCVGraph>() };
			if (graph && eSuccess == graph->runGraph())
			{
				algorithms.insert(am.algos[0].id, graph);

				//׼���㷨��ز���
				std::vector<std::string> params;
				boost::split(params, am.algos[0].camera, boost::is_any_of(":"));
				const std::string streamID{ (boost::format("%s:%s") % params[4] % params[5]).str() };
				const std::size_t sidsize{ streamID.length() };
				CVParam cvp;
				memset(&cvp, 0, sizeof(CVParam));
#ifdef WINDOWS
				memcpy_s(cvp.ownID, sidsize, streamID.c_str(), sidsize);
#else
				memcpy(cvp.ownID, streamID.c_str(), sidsize);
#endif
				cvp.gpuID = am.algos[0].gpu;
				cvp.detectThreshold = am.algos[0].detect;
				cvp.trackThreshold = am.algos[0].track;

				//����������ݽ��ջص�����
				FilterPtr fp{
					graph->queryFilter(framework::multimedia::JPEGEncoderFilterName) };
				if (fp)
				{
					fp->setDataNotificationCallback(boost::bind(&Phone::afterEncodeJPEGImageCallback, this, _1));
				}

				//��ʼ���㷨
				fp = graph->queryFilter(framework::multimedia::CVCompuptingFilterName);
				if (fp)
				{
					boost::shared_ptr<CVComputingFilter> cvfp{
						boost::dynamic_pointer_cast<CVComputingFilter>(fp) };
					cvfp->initCV(&cvp);

					// 						fp = graph->queryFilter(framework::multimedia::TCPDataReceiverFilterName);
					// 						boost::shared_ptr<TCPDataReceiverFilter> drfp{ 
					// 							boost::dynamic_pointer_cast<TCPDataReceiverFilter>(fp) };
					// 						boost::asio::ip::tcp::socket so{ ASIO::getTCPSocket() };
					// 						if (drfp && drfp->start(am.algos[0].camera, so))
					// 						{
					// 							algorithms.insert(am.algos[0].id, graph);
					// 						}
					// 						else
					// 						{
					// 							e = eBadOperate;
					// 						}
				}

				//����Ƶ����
				fp = graph->queryFilter(framework::multimedia::HKNetSDKFilterName);
				if (fp)
				{
					HKNetSDKFilterPtr sdkfp{
						boost::dynamic_pointer_cast<HKNetSDKFilter>(fp) };
					sdkfp->start(
						params[0].c_str(), atoi(params[1].c_str()), params[2].c_str(), params[3].c_str(), atoi(params[5].c_str()));
				}
			}
			else
			{
				e = eBadOperate;
			}
		}
		else if (2 == am.command)
		{
			//DELETE_REQ
		}

		LOG(INFO) << "Phone process algorithm message with module [  " << module <<
			" ] from [ " << from <<
			" ] to [ " << to <<
			" ], sequence = " << sequence <<
			", timestamp = " << timestamp <<
			", type = [ " << am.algos[0].type <<
			", gpu = " << am.algos[0].gpu <<
			", id = " << am.algos[0].id <<
			", camera = " << am.algos[0].camera <<
			", detect = " << am.algos[0].detect <<
			", track = " << am.algos[0].track <<
			", detect2forphone = " << am.algos[0].detect2forphone <<
			", similarforface = " << am.algos[0].similarforface <<
			", triggertimeforsleep = " << am.algos[0].triggertimeforsleep <<
			", inoutforface = " << am.algos[0].inoutforface << ".";
	}
	else
	{
		LOG(WARNING) << "Phone process algorithm message with module [  " << module <<
			" ] from [ " << from <<
			" ] to [ " << to <<
			" ], sequence = " << sequence <<
			", timestamp = " << timestamp <<
			" failed, result = " << e << ".";
	}

	am.status = e;
	++am.command;

	//----------------------------------------------------------------------------------------------
	//| "" | "worker" | "web" | "cms" | sender | "" | sequence | timestamp | "algorithm" | message |
	//----------------------------------------------------------------------------------------------
	Msg msg_;
	msg_.addMessage(ProtocolAlgorithm().pack(am));
	msg_.addMessage(gAlgorithmCommandName);
	msg_.addMessage(timestamp);
	msg_.addMessage(sequence);
	msg_.addMessage(gEmptyData);
	//���Ͷ�ID��ʶ
	msg_.addMessage(routers[0]);
	msg_.addMessage(to);
	msg_.addMessage(from);
	msg_.addMessage(module);
	msg_.addMessage(gEmptyData);

	if (eSuccess == OD::sendMsg(&msg_))
	{
		LOG(INFO) << "Phone send message of device successfully.";
	}
	else
	{
		LOG(WARNING) << "Phone send message of device failed.";
	}
}

void Phone::processStatusMessage(
	const std::string module,
	const std::string from,
	const std::string to,
	const std::vector<std::string> routers,
	const std::string sequence,
	const std::string timestamp,
	const std::string msg)
{
	using StatusMsg = framework::data::StatusMsg;
	StatusMsg sm;
	int e{ ProtocolStatus().unpack(msg, sm) };

	if (eSuccess == e)
	{
		if (1 == sm.command)
		{
			//SET_REQ

			workState = (1 == sm.state ? true : false);
		}

		LOG(INFO) << "Phone process status message with module [  " << module <<
			" ] from [ " << from <<
			" ] to [ " << to <<
			" ], sequence = " << sequence <<
			", timestamp = " << timestamp <<
			", state = [ " << sm.state << " ].";
	}
	else
	{
		LOG(WARNING) << "Phone process status message with module [  " << module <<
			" ] from [ " << from <<
			" ] to [ " << to <<
			" ], sequence = " << sequence <<
			", timestamp = " << timestamp <<
			" failed, result = " << e << ".";
	}

	sm.status = e;
	++sm.command;

	//-------------------------------------------------------------------------------------------
	//| "" | "worker" | "web" | "cms" | sender | "" | sequence | timestamp | "status" | message |
	//-------------------------------------------------------------------------------------------
	Msg msg_;
	msg_.addMessage(ProtocolStatus().pack(sm));
	msg_.addMessage(gStatusCommandName);
	msg_.addMessage(timestamp);
	msg_.addMessage(sequence);
	msg_.addMessage(gEmptyData);
	//���Ͷ�ID��ʶ
	msg_.addMessage(routers[0]);
	msg_.addMessage(to);
	msg_.addMessage(from);
	msg_.addMessage(module);
	msg_.addMessage(gEmptyData);

	if (eSuccess == OD::sendMsg(&msg_))
	{
		LOG(INFO) << "Phone send message of device successfully.";
	}
	else
	{
		LOG(WARNING) << "Phone send message of device failed.";
	}
}

void Phone::afterEncodeJPEGImageCallback(DataPtr data)
{
	if (workState)
	{
		FramePtr fp{ 
			boost::dynamic_pointer_cast<Frame>(data) };
		framework::data::OT ot{ fp->getOT() };

		framework::data::AlarmMsg almmsg;
		almmsg.command = 1;
		almmsg.alarm.inoutforface = false;
		almmsg.alarm.type = 2;
		almmsg.alarm.bodycount = 0;
		almmsg.alarm.x = ot.otps[0].x;
		almmsg.alarm.y = ot.otps[0].y;
		almmsg.alarm.w = ot.otps[0].w;
		almmsg.alarm.h = ot.otps[0].h;
		almmsg.alarm.camera = fp->getFrameID();
		almmsg.alarm.timestamp = "1970-01-01 00:00:00 +08";
		almmsg.alarm.picture.append((const char*)data->getData());
		almmsg.alarm.idforface = "";

		//---------------------------------------------------------------------------------------------
		//| "" | "worker" | "helmet" | "cms" | sender | "" | sequence | timestamp | "alarm" | message |
		//---------------------------------------------------------------------------------------------
		Msg msg_;
		msg_.addMessage(ProtocolAlarm().pack(almmsg));
		msg_.addMessage(gAlarmCommandName);
		msg_.addMessage((boost::format("%d") % (int)Time().tickcount()).str());
		msg_.addMessage((boost::format("%d") % ++alarmSequenceNumber).str());
		msg_.addMessage(gEmptyData);
		msg_.addMessage(gCMSComponentName);
		msg_.addMessage(gPhoneComponentName);
		msg_.addMessage(gWorkerModuleName);
		msg_.addMessage(gEmptyData);

		if (eSuccess == OD::sendMsg(&msg_))
		{
			LOG(INFO) << "Phone send alarm message successfully.";
		}
		else
		{
			LOG(WARNING) << "Phone send alarm message failed.";
		}
	}
}
