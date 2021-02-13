//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-12-18
//		Description :					��Դ���������
//
//		History:						Author									Date										Description
//										������									2020-12-18									����
//

#ifndef DHS_H
#define DHS_H

#include "DataStruct/UnorderedMap.h"
#include "Data/Protocol/ProtocolComponent.h"
using ProtocolComponent = framework::data::ProtocolComponent;
using ComponentMsg = framework::data::ComponentMsg;
#include "Network/ZMQ/OD.h"
using OD = framework::network::zeromq::OD;

class CMS final : public OD
{
public:
	CMS(
		const std::string name, 
		const std::string id);
	~CMS(void);

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

private:
	void sendPairMessageThread(void);
	void checkRegisterComponentTimeoutThread(void);
	void processXMQPairMessage(
		const std::string module,
		const std::string from,
		const std::string to,
		const std::string sequence,
		const std::string timestamp,
		const std::string msg);
	void processComponentMessage(
		const std::string module,
		const std::string from,
		const std::string to,
		const std::vector<std::string> routers,
		const std::string sequence,
		const std::string timestamp,
		const std::string msg);
	void processDeviceMessage(
		const std::string module,
		const std::string from,
		const std::string to,
		const std::vector<std::string> routers,
		const std::vector<std::string> messages);
	void processClockMessage(
		const std::string module,
		const std::string from,
		const std::string to,
		const std::vector<std::string> routers,
		const std::vector<std::string> messages);
	void processAlgorithmMessage(
		const std::string module,
		const std::string from,
		const std::string to,
		const std::vector<std::string> routers,
		const std::vector<std::string> messages);
	void processStatusMessage(
		const std::string module,
		const std::string from,
		const std::string to,
		const std::vector<std::string> routers,
		const std::vector<std::string> messages);
	void processCrewMessage(
		const std::string module,
		const std::string from,
		const std::string to,
		const std::vector<std::string> routers,
		const std::vector<std::string> messages);
	void processAlarmMessage(
		const std::string module,
		const std::string from,
		const std::string to,
		const std::vector<std::string> routers,
		const std::vector<std::string> messages);

private:
	const std::string cmsName;
	const std::string cmsID;
	int pairSequenceNumber;

	typedef struct tagRegisterComponent_t
	{
		unsigned long long timestamp;//ע��ʱ��
		std::vector<std::string> routers;
		ComponentMsg component;
	}RegisterComponent;
	//ע���������
	//KEY�����ID��ʶ
	using RegisterComponentGroup = UnorderedMap<const std::string, RegisterComponent>;
	RegisterComponentGroup registerComponentGroup;
};//class CMS

#endif//CMS_H
