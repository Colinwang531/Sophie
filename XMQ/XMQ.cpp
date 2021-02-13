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
#include "Network/ZMQ/Core/Router_.h"
#include "Network/ZMQ/Core/Dealer_.h"
#include "Network/ZMQ/Msg_.h"
using Msg = framework::network::zeromq::Msg;
#include "XMQ.h"

XMQ::XMQ() : RD()
{}
XMQ::~XMQ()
{}

int XMQ::startRD(
	const unsigned short rlp /* = 0 */, 
	const char* dcip /* = nullptr */, 
	const unsigned short dcp /* = 0 */)
{
	LOG(INFO) << "Starting XMQ with parameters of command line [ Listen port = " << rlp <<
		", remote XMQ IPv4 = " << dcip <<
		", remote XMQ port = " << dcp << " ].";

	int e{ RD::startRD(rlp, dcip, dcp) };

	if (eSuccess == e)
	{
		LOG(INFO) << "Starting XMQ service successfully.";
	}
	else
	{
		LOG(WARNING) << "Starting XMQ service failed, result = " << e << ".";
	}

	return e;
}

int XMQ::stopRD()
{
	int e{ RD::stopRD() };

	if (eSuccess == e)
	{
		LOG(INFO) << "Stopping XMQ service successfully.";
	}
	else
	{
		LOG(WARNING) << "Stopping XMQ service failed, result = " << e << ".";
	}

	return e;
}

void XMQ::afterParsePolledMessage(
	const std::string sender,
	const std::string module,
	const std::string from,
	const std::string to,
	const std::vector<std::string> routers,
	const std::vector<std::string> messages)
{
	//Step1:�鿴·�ɱ��Ƿ�Ϊ��?
	//      ������ȡ��·�ɱ����׸�·��IDת�����ݲ���·�ɱ���ɾ����ID;
	//		���ǡ���Step2;
	//Step2:�鿴to�Ƿ���"xmq"?
	//		"��"��Step3;
	//		"��"����;
	//Step3:�鿴to�Ƿ���"cms"?
	//		"��"��Step4;
	//		"��"��ת��;
	//Step4:�鿴to�Ƿ���"uplink"?
	//		"��"��Step5;
	//		"��"��ͨ��MDCģ��ת��;
	//Step5:�ظ���Ϣ���ɴ�,ֱ�Ӷ���

	if (0 < routers.size())
	{
		//Step1
//		std::vector<const std::string> routers{ routers };
		forwardPolledMessage(routers[0], 1 < routers.size() ? routers[1] : "", module, from, to, messages);
	} 
	else
	{
		if (!to.compare(gXMQComponentName))
		{
			//Step2
			processPolledMessage(sender, module, from, to, messages);
		} 
		else if (!to.compare(gCMSComponentName))
		{
			//Step3
			forwardPolledMessage(cmsCommID, sender, module, from, to, messages);
		}
		else if (!to.compare(gUplinkComponentName))
		{
			//Step4
		}
		else
		{
			//Step5
		}
	}
}

void XMQ::afterParsePolledMessage(
	const std::string module,
	const std::string from,
	const std::string to,
	const std::vector<std::string> routers,
	const std::vector<std::string> messages)
{
}

void XMQ::processPolledMessage(
	const std::string sender,
	const std::string module,
	const std::string from,
	const std::string to,
	const std::vector<std::string> messages)
{
	//��Ϣ���ݵ����2������һ����command��message
	const std::size_t msgDataNumber{ messages.size() };
	const std::string sequence{ messages[msgDataNumber - 4] };
	const std::string timestamp{ messages[msgDataNumber - 3] };
	const std::string command{ messages[msgDataNumber - 2] };
	const std::string message{ messages[msgDataNumber - 1] };

	//����CMS�����Ϣ
	if (!command.compare(gPairCommandName))
	{
		processCMSPairMessage(sender, module, from, to, sequence, timestamp);
	}
}

void XMQ::forwardPolledMessage(
	const std::string receiver,
	const std::string sender,
	const std::string module,
	const std::string from,
	const std::string to,
	const std::vector<std::string> messages)
{
	//--------------------------------------------------------------------------------------------------------------------
	//| receiver | "" | module | from | to | "" | extend(1) | ... | extend(n) | sequence | timestamp | command | message |
	//--------------------------------------------------------------------------------------------------------------------
	Msg msg;
	int msgNum{ static_cast<int>(messages.size()) };
	for (int i = msgNum; i != 0; --i)
	{
		msg.addMessage(messages[i - 1]);
	}
	msg.addMessage(gEmptyData);
	if (!sender.empty())
	{
		msg.addMessage(sender);
	}
	msg.addMessage(to);
	msg.addMessage(from);
	msg.addMessage(module);
	msg.addMessage(gEmptyData);
	msg.addMessage(receiver);
	int e{ RD::sendMsg(0, &msg) };

	if (eSuccess == e)
	{
		LOG(INFO) << "XMQ forward message from " << from << " to " << to << " successfully.";
	}
	else
	{
		LOG(WARNING) << "XMQ forward message from " << from << " to " << to << " failed, result = " << e << ".";
	}
}

void XMQ::processCMSPairMessage(
	const std::string sender, 
	const std::string module, 
	const std::string from, 
	const std::string to,
	const std::string sequence,
	const std::string timestamp)
{
	int e{ 
		!sender.empty() && !module.empty() && !from.empty() && !to.empty() ? eSuccess : eInvalidParameter };

	if (eSuccess == e && !from.compare(gCMSComponentName) && !to.compare(gXMQComponentName))
	{
		//ÿ����Զ�����CMS��ͨ��ID��ʶ
		cmsCommID = sender;
		LOG(INFO) << "XMQ process pair command from [ " << from <<
			" ] as ID = " << sender << 
			", sequence = " << sequence << 
			", timestamp = " << timestamp << ".";
	} 
	else
	{
		e = eInvalidParameter;
		LOG(WARNING) << "XMQ process invalid pair command from [ " << from <<
			" ] to [ " << to <<
			" ] as ID = " << sender <<
			", sequence = " << sequence <<
			", timestamp = " << timestamp << ".";
	}

	//----------------------------------------------------------------------------------
	//| sender | "" | module | to | from | "" | sequence | timestamp | "pair" | Status |
	//----------------------------------------------------------------------------------
	Msg msg;
	msg.addMessage((boost::format("%d") % e).str());
	msg.addMessage(gPairCommandName);
	msg.addMessage(timestamp);
	msg.addMessage(sequence);
	msg.addMessage(gEmptyData);
	msg.addMessage(from);
	msg.addMessage(to);
	msg.addMessage(module);
	msg.addMessage(gEmptyData);
	msg.addMessage(sender);
	e = RD::sendMsg(0, &msg);

	if (eSuccess == e)
	{
		LOG(INFO) << "XMQ send status message of pairing successfully.";
	}
	else
	{
		LOG(WARNING) << "XMQ send status message of pairing failed, result = " << e << ".";
	}
}

// void XMQ::transferParsedMessageFromMDS(
// 	const std::string sender, 
// 	const std::string module, 
// 	const std::string from, 
// 	const std::string to, 
// 	const std::vector<const std::string> routers, 
// 	const std::vector<const std::string> messages)
// {
// 	const bool isFromCMS{ !fromID.compare(gCMSComponentName) ? true : false };
// 
// 	//ͨ��fromID����ע����Ϣ��Դ
// 	//CMS���ע����Workerģ�ʹ���,������������Brokerģ�ʹ���
// 	//��Ϣ��ʽ����
// 	//-----------------------------------------------------------------------------
// 	//| CommID | Empty | "worker" | "..." | "cms" | "register" | Name:UUID:CommID |
// 	//-----------------------------------------------------------------------------
// 	MQMessagePacket msg;
// 	msg.addMessage((boost::format("%s:%s") % data % commID).str());
// 	msg.addMessage(gRegisterCommandName);
// 	msg.addMessage(viaID);
// 	//�����XMQת����Ϣ��Ҫ�޸�fromID��ʶΪ"xmq"
// 	msg.addMessage(isFromCMS ? gXMQComponentName : fromID);
// 	msg.addMessage(gWorkerModuleName);
// 	msg.addMessage(gEmptyCommandName);
// 
// 	int e{ eCannotReach };
// 	if (isFromCMS)
// 	{
// 		//��XMQת��
// 		if (worker)
// 		{
// 			e = worker->sendData(msg);
// 		}
// 	} 
// 	else
// 	{
// 		//��CMSת��
// 		msg.addMessage(commIDOfCMS);
// 		e = broker->sendData(msg);
// 	}
// 
// 	//����ʧ��������Ӧ��
// 	if (eSuccess != e)
// 	{
// 		//��Ϣ��ʽ����
// 		//-------------------------------------------------------------------
// 		//| CommID | Empty | "worker" | "..." | "cms" | "register" | Status |
// 		//-------------------------------------------------------------------
// 		MQMessagePacket msg;
// 		msg.addMessage((boost::format("%d") % e).str());
// 		msg.addMessage(gRegisterCommandName);
// 		msg.addMessage(viaID);
// 		msg.addMessage(fromID);
// 		msg.addMessage(moduleID);
// 		msg.addMessage(gEmptyCommandName);
// 		msg.addMessage(commID);
// 
// 		int e{ broker->sendData(msg) };
// 
// 		if (eSuccess == e)
// 		{
// 			LOG(INFO) << "XMQ send status of register message successfully.";
// 		}
// 		else
// 		{
// 			LOG(WARNING) << "XMQ send status of register message failed, result = " << e << ".";
// 		}
// 	}
// }
