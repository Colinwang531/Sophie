//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-06-15
//		Description :					海康设备组件客户端类
//
//		History:						Author									Date										Description
//										王科威									2020-05-13									创建
//

#include "boost/shared_ptr.hpp"
#include "Packet/DataPacket.h"
using DataPacket = base::packet::DataPacket;
using DataPacketPtr = boost::shared_ptr<DataPacket>;
#include "Device/Hikvision/HikvisionDevice.h"
using AbstractCamera = base::device::AbstractCamera;
using AbstractDevice = base::device::AbstractDevice;
using SurveillanceDevice = base::device::SurveillanceDevice;
using HikvisionDevice = base::device::HikvisionDevice;
using AbstractDevicePtr = boost::shared_ptr<AbstractDevice>;
using SurveillanceDevicePtr = boost::shared_ptr<SurveillanceDevice>;
#include "DataStruct/UnorderedMap.h"
using AbstractDeviceGroup = UnorderedMap<const std::string, AbstractDevicePtr>;
#include "HKDMediaStreamSession.h"
using TCPSessionPtr = boost::shared_ptr<TCPSession>;
using HKComponentStreamSessionGroup = UnorderedMap<const std::string, TCPSessionPtr>;
#include "Network/AbstractMediaStreamClient.h"
using AbstractMediaStreamClient = base::network::AbstractMediaStreamClient;

class HKDComponentClient : public AbstractMediaStreamClient
{
public:
	HKDComponentClient(
		const std::string address,
		const unsigned short port = 60531);
	virtual ~HKDComponentClient(void);

protected:
	void afterClientPolledMessageProcess(
		const std::string flagID,
		const std::string fromID,
		const std::string toID,
		const std::string msg) override;
	const std::string buildAutoRegisterToBrokerMessage(void) override;
	int createNewMediaStreamSession(
		const std::string url, 
		boost::asio::ip::tcp::socket* s) override;

private:
	const std::string getHKDClientInfoByName(const std::string name) const;
	void setHKDClientInfoWithName(
		const std::string name, 
		const std::string value);
	void processComponentMessage(DataPacketPtr pkt);
	void processDeviceMessage(
		const std::string fromID, 
		DataPacketPtr pkt);
	void processEventMessage(
		const std::string fromID,
		DataPacketPtr pkt);
	int processMediaStream(
		const int command, 
		const std::string did,
		const std::vector<AbstractCamera> cameras);

	int addNewDevice(
		SurveillanceDevice* sd, 
		std::vector<AbstractCamera>& cameras);
	int deleteDeviceByID(const std::string did);
	int replyMessageWithResult(
		const std::string fromID,
		const int command,
		const int result,
		const long long sequence,
		const std::vector<AbstractCamera>& cameras,
		const std::string did);

private:
	AbstractDeviceGroup deviceGroup;
	HKComponentStreamSessionGroup streamSessionGroup;
};//class HKDComponentClient

