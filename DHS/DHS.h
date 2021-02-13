//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-06-15
//		Description :					视频监控设备主机类
//
//		History:						Author									Date										Description
//										王科威									2020-05-13									创建
//										王科威									2020-12-07									重构视频监控设备主机类设计
//

#ifndef DHS_H
#define DHS_H

#include "boost/shared_ptr.hpp"
#include "Device/Device.h"
using Device = framework::device::Device;
#include "DataStruct/UnorderedMap.h"
using DevicePtr = boost::shared_ptr<Device>;
using DeviceGroup = UnorderedMap<const std::string, DevicePtr>;
#include "Network/ASIO/Session/TCPSession.h"
using TCPSession = framework::network::asio::TCPSession;
using TCPSessionPtr = boost::shared_ptr<TCPSession>;
#include "Network/Pattern/XMQPeer.h"
using XMQPeer = framework::network::pattern::XMQPeer;

class DHS final : public XMQPeer
{
public:
	DHS(
		const std::string name, 
		const std::string id,
		const unsigned short port = 0);
	~DHS(void);

public:
	int startOD(
		const char* ipv4 = nullptr,
		const unsigned short port = 0) override;
	int stopOD(void) override;

private:
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
	void processDeviceMessage(
		const std::string module,
		const std::string from,
		const std::string to,
		const std::vector<std::string> routers,
		const std::string sequence,
		const std::string timestamp,
		const std::string msg);

private:
	void afterTCPAcceptResultCallback(
		const int e,
		boost::asio::ip::tcp::socket& so);
	void afterTCPTargetReceiveResultCallback(
		const char* data,
		const int bytes,
		TCPSessionPtr tsp);

private:
	const unsigned short mediaPort;
	DeviceGroup devices;
};//class DHS

#endif//DHS_H
