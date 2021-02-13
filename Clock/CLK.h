//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-03
//		Description :					母钟组件客户端类
//
//		History:						Author									Date										Description
//										王科威									2020-10-03									创建
//

#ifndef CLK_H
#define CLK_H

#include "Network/Pattern/XMQPeer.h"
using XMQPeer = framework::network::pattern::XMQPeer;
#include "Hardware/Com.h"
using Com = framework::hardware::Com;

class CLK final
	: public XMQPeer, protected Com
{
public:
	CLK(
		const std::string name,
		const std::string id,
		const int number = -1,
		const int br = 4800);
	~CLK(void);

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
	void afterReceiveComDataNotification(
		const char* data = nullptr,
		const int bytes = 0) override;
	const std::string buildRegisterMessage(void) override;

private:
	//发送时钟消息
	//@Return : 消息
	int sendClockMessage(const std::string clock);

	void processComponentMessage(
		const std::string module,
		const std::string from,
		const std::string to,
		const std::string sequence,
		const std::string timestamp,
		const std::string msg);

private:
	const int comNumber;
	const int baudRate;
	std::string comData;
	int clockSequenceNumber;
};//class AIS

#endif // !CLK_H
