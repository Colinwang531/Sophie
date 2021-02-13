//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-03
//		Description :					AIS����ͻ�����
//
//		History:						Author									Date										Description
//										������									2020-10-03									����
//

#ifndef AIS_H
#define AIS_H

#include "Network/Pattern/XMQPeer.h"
using XMQPeer = framework::network::pattern::XMQPeer;
#include "Hardware/Com.h"
using Com = framework::hardware::Com;
#include "Decoder/Decoder.h"
using Decoder = framework::multimedia::Decoder;
#include "Data/Protocol/ProtocolAIS.h"
using AISMsg = framework::data::AISMsg;

class AIS final 
	: public XMQPeer, protected Com, protected Decoder
{
public:
	AIS(
		const std::string name, 
		const std::string id, 
		const int number = -1, 
		const int br = 38400);
	~AIS(void);

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
	void afterDataDecodeNotification(DataPtr data) override;

private:
	//����AIS��Ϣ
	//@Return : ��Ϣ
	int sendAISMessage(const AISMsg& ais);

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
	std::string parseData;
	int aisSequenceNumber;
};//class AIS

#endif // !AIS_H
