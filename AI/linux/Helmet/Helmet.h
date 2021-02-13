//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-01-21
//		Description :					��ȫñ�㷨��
//
//		History:						Author									Date										Description
//										������									2021-01-21									����
//

#ifndef HELMET_DETECT_H
#define HELMET_DETECT_H

#include "boost/shared_ptr.hpp"
#include "DataStruct/UnorderedMap.h"
#include "Graph/Graph.h"
using Graph = framework::multimedia::Graph;
using GraphPtr = boost::shared_ptr<Graph>;
#include "Data/Data.h"
using Data = framework::data::Data;
using DataPtr = boost::shared_ptr<Data>;
#include "Network/Pattern/XMQPeer.h"
using XMQPeer = framework::network::pattern::XMQPeer;

class Helmet final : public XMQPeer
{
public:
	Helmet(
		const std::string name,
		const std::string id);
	~Helmet(void);

public:
	int startOD(
		const char* ipv4 = nullptr,
		const unsigned short port = 0) override;
	int stopOD(void) override;

protected:
	void afterParsePolledMessage(
		const std::string module,
		const std::string from,
		const std::string to,
		const std::vector<std::string> routers,
		const std::vector<std::string> messages) override;
	const std::string buildRegisterMessage(void) override;

	void processComponentMessage(
		const std::string module,
		const std::string from,
		const std::string to,
		const std::string sequence,
		const std::string timestamp,
		const std::string msg);
	void processAlgorithmMessage(
		const std::string module,
		const std::string from,
		const std::string to,
		const std::vector<std::string> routers,
		const std::string sequence,
		const std::string timestamp,
		const std::string msg);
	void processStatusMessage(
		const std::string module,
		const std::string from,
		const std::string to,
		const std::vector<std::string> routers,
		const std::string sequence,
		const std::string timestamp,
		const std::string msg);

private:
	void afterEncodeJPEGImageCallback(DataPtr data);

private:
	bool workState;
	using AlgorithmGroup = UnorderedMap<std::string, GraphPtr>;
	AlgorithmGroup algorithms;
	int alarmSequenceNumber;
};//class Helmet

#endif//HELMET_DETECT_H
