#include <new>
#include "libcommon/const.h"
#include "libcommon/error.h"
#include "libnetwork/zmq/ctx.h"
using Ctx = framework::libnetwork::zmq::Ctx;
#include "XMQ.h"

XMQ::XMQ(Log& log_) : Dispatcher(), bindPort{0}, connectPort{0}, ctx{new(std::nothrow) Ctx}, log{log_}
{}
XMQ::XMQ(
	const std::string localIP,
	const unsigned short localPort,
	const std::string uplinkIP,
	const unsigned short uplinkPort) 
	: Dispatcher(), bindIP{localIP}, bindPort{localPort}, 
	connectIP{uplinkIP}, connectPort{uplinkPort}, ctx{new(std::nothrow) Ctx}, log{log_}
{}
XMQ::~XMQ()
{
	Ctx* c{reinterpret_cast<Ctx*>(ctx)};
	if (c)
	{
		delete c;
	}
}

int XMQ::start()
{
	CommonError e{
		ctx ? COMMON_ERROR_SUCCESS : COMMON_ERROR_BAD_NEW_INSTANCE};

	if (CommonError::COMMON_ERROR_SUCCESS == e)
	{
		//绑定Router端监听
		if (!bindIP.empty() && gMinPortNumber <= bindPort && gMaxPortNumber >= bindPort)
		{
			e = static_cast<CommonError>(Dispatcher::bind(bindIP, bindPort, ctx));
			log.write(
				framework::liblog::LogLevel::LOG_LEVEL_INFO, 
				"XMQ bind local IP [ %s ] and PORT [ %d ] result = [ %d ].", 
				bindIP.c_str(), bindPort, static_cast<int>(e));
		}

		//连接Dealer端连接
		if (!connectIP.empty() && gMinPortNumber <= connectPort && gMaxPortNumber >= connectPort)
		{
			e = static_cast<CommonError>(Dispatcher::connect(connectIP, connectPort, ctx));
			log.write(
				framework::liblog::LogLevel::LOG_LEVEL_INFO, 
				"XMQ connect remote IP [ %s ] and PORT [ %d ] result = [ %d ].", 
				connectIP.c_str(), connectPort, static_cast<int>(e));
		}

		//处理完Router和Dealer后才能启动Dispatcher服务
		//Dispatcher服务创建线程接收Router和Dealer的数据
		e = static_cast<CommonError>(Dispatcher::start());
		log.write(
			framework::liblog::LogLevel::LOG_LEVEL_INFO, 
			"XMQ start service result = [ %d ].",
			static_cast<int>(e));
	}
	else
	{
		log.write(
			framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
			"XMQ start service failed with invalid object of context.");
	}

	return static_cast<int>(e);
}

int XMQ::stop(void* /* = nullptr*/)
{
	CommonError e{
		ctx ? COMMON_ERROR_SUCCESS : COMMON_ERROR_BAD_NEW_INSTANCE};

	if (CommonError::COMMON_ERROR_SUCCESS == e)
	{
		e = static_cast<CommonError>(Dispatcher::stop(ctx));
		log.write(
			framework::liblog::LogLevel::LOG_LEVEL_INFO, 
			"XMQ stop service result = [ %d ].",
			static_cast<int>(e));
	}
	else
	{
		log.write(
			framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
			"XMQ stop service failed with invalid object of context.");
	}

	return static_cast<int>(e);
}

void XMQ::afterRouterPollDataProcess(
	const std::string sender,
	const std::string data)
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

void XMQ::afterDealerPollDataProcess(
	const std::string data)
{
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

int XMQ::updatePairedCMSID(const std::string id)
{
	CommonError e{ 
		id.empty() ? CommonError::COMMON_ERROR_INVALID_PARAMETER : CommonError::COMMON_ERROR_SUCCESS };

	if (CommonError::COMMON_ERROR_SUCCESS == e)
	{
		pairedCMSID = id;
		log.write(
			framework::liblog::LogLevel::LOG_LEVEL_INFO, 
			"Update CMS component identity [ %s ] successfully.",
			pairedCMSID.c_str());
	} 
	else
	{
		log.write(
			framework::liblog::LogLevel::LOG_LEVEL_INFO, 
			"Update CMS component identity [ %s ] failed, result = [ %d ].",
			id.c_str(), static_cast<int>(e));
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
