//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-13
//		Description :					XMQ类
//
//		History:						Author									Date										Description
//										王科威									2020-05-13									创建
//										王科威									2020-11-25									1.重构消息分发服务抽象设计
//										     									          									2.修改CMS组件连接检测线程为业务逻辑线程
//

#ifndef XMQ_H
#define XMQ_H

//#include "boost/shared_ptr.hpp"
// #include "MessageQueue/MQMessagePacket.h"
// using MQMessagePacket = mq::message::MQMessagePacket;
// #include "MessageQueue/MajordomoBroker.h"
// using MajordomoBroker = mq::module::MajordomoBroker;
// using MajordomoBrokerPtr = boost::shared_ptr<MajordomoBroker>;
// #include "MessageQueue/MajordomoWorker.h"
// using MajordomoWorker = mq::module::MajordomoWorker;
// using MajordomoWorkerPtr = boost::shared_ptr<MajordomoWorker>;
#include "Network/ZMQ/RD.h"
using RD = framework::network::zeromq::RD;
// #include "Network/AbstractServer.h"
// using AbstractServer = base::network::AbstractServer;
// #include "Network/AbstractWorker.h"
// using AbstractWorker = base::network::AbstractWorker;

class XMQ final : public RD
{
public:
	XMQ(void);
	~XMQ(void);

public:
	int startRD(
		const unsigned short rlp = 0,
		const char* dcip = nullptr,
		const unsigned short dcp = 0) override;
	int stopRD(void) override;

protected:
	void afterParsePolledMessage(
		const std::string sender,
		const std::string module,
		const std::string from,
		const std::string to,
		const std::vector<std::string> routers,
		const std::vector<std::string> messages) override;
	void afterParsePolledMessage(
		const std::string module,
		const std::string from,
		const std::string to,
		const std::vector<std::string> routers,
		const std::vector<std::string> messages) override;

private:
	//处理消息
	void processPolledMessage(
		const std::string sender,
		const std::string module,
		const std::string from,
		const std::string to, 
		const std::vector<std::string> messages);

	//处理CMS配对消息
	//@commID : 发送者ID标识
	//@module : 发送者模型标识
	//@from : 发送者名称
	//@to : 接收者名称
	//@Comment : 应答配对状态
	void processCMSPairMessage(
		const std::string sender,
		const std::string module,
		const std::string from,
		const std::string to,
		const std::string sequence,
		const std::string timestamp);

	//转发消息
	//@receiver : 接收者ID标识
	//@receiver : 发送者ID标识
	//@module : 模型标识
	//@from : 发送者名称
	//@to : 接收者名称
	//@messages : 消息内容
	void forwardPolledMessage(
		const std::string receiver,
		const std::string sender,
		const std::string module,
		const std::string from,
		const std::string to,
		const std::vector<std::string> messages);

private:
	std::string cmsCommID;
};//class XMQ

#endif//XMQ_H
