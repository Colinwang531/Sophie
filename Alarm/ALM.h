//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-03
//		Description :					报警推送组件类
//
//		History:						Author									Date										Description
//										王科威									2020-10-03									创建
//

#ifndef ALM_H
#define ALM_H

#include "Network/ZMQ/Pub_.h"
using Pub = framework::network::zeromq::Pub;
#include "Network/Pattern/XMQPeer.h"
using XMQPeer = framework::network::pattern::XMQPeer;

class ALM final : public XMQPeer, protected Pub
{
public:
	ALM(
		const std::string name,
		const std::string id,
		const unsigned short port = 60820);
	~ALM(void);

public:
	int startOD(
		const char* ipv4 = nullptr,
		const unsigned short port = 0);
	int stopOD(void);

protected:
	void afterParsePolledMessage(
		const std::string module,
		const std::string from,
		const std::string to,
		const std::vector<std::string> routers,
		const std::vector<std::string> messages) override;
	const std::string buildRegisterMessage(void) override;

private:
	void processComponentMessage(
		const std::string module,
		const std::string from,
		const std::string to,
		const std::string sequence,
		const std::string timestamp,
		const std::string msg);
	void processClockMessage(
		const std::string module,
		const std::string from,
		const std::string to,
		const std::string sequence,
		const std::string timestamp,
		const std::string msg);
	void processAlarmMessage(
		const std::string module,
		const std::string from,
		const std::string to,
		const std::string sequence,
		const std::string timestamp,
		const std::string msg);

private:
	const unsigned short pubPort;
	std::string clock;
};//class ALM

#endif//ALM_H

