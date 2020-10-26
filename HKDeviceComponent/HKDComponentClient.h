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
#include "DataStruct/Vector.h"
#include "MessageQueue/MajordomoWorker.h"
using MajordomoWorker = mq::module::MajordomoWorker;
using MajordomoWorkerPtr = boost::shared_ptr<MajordomoWorker>;
#include "ASIO/ASIOService.h"
using ASIOService = base::network::ASIOService;
#include "HKDMediaStreamSession.h"
using TCPSessionPtr = boost::shared_ptr<TCPSession>;
using HKComponentStreamSessionGroup = UnorderedMap<const std::string, TCPSessionPtr>;
#include "Network/AbstractWorker.h"
using AbstractWorker = base::network::AbstractWorker;

class HKDComponentClient : public AbstractWorker
{
public:
	HKDComponentClient(void);
// 		const std::string address,
// 		const unsigned short port = 60531);
	virtual ~HKDComponentClient(void);

protected:
	int createNewClient(const std::string address) override;
	int destroyClient(void) override;
	//WORKER端数据读取回调
	//@roleID : 角色ID标识
	//@flagID : 标志ID标识
	//@fromID : 发送者ID标识
	//@toID : 接收者ID标识
	//@data : 消息数据
	//@Return : 错误码
	void afterPolledDataFromWorkerCallback(
		const std::string roleID,
		const std::string flagID,
		const std::string fromID,
		const std::string toID,
		const std::string data);
	void sendRegisterWorkerServerMessage(void) override;
	int sendData(
		const std::string roleID,
		const std::string flagID,
		const std::string fromID,
		const std::string toID,
		const std::string data) override;
	int createNewMediaStreamSession(
		const std::string url, 
		boost::asio::ip::tcp::socket* s);

private:
	//连接服务端
	//@did : 设备ID标识
	//@cid : 摄像机ID标识
	//@idx : 摄像机索引
	//@Return : 错误码
	int connectMediaServer(
		const std::string did,
		const std::string cid,
		const int idx = -1);

	//断开服务端连接
	//@did : 设备ID标识
	//@Return : 错误码
	//@Comment : 断开设备上所有的连接
	int disconnectMediaServer(const std::string did);

private:
	void afterRemoteConnectedNotificationCallback(
		boost::asio::ip::tcp::socket* s,
		const boost::system::error_code e);

	const std::string getHKDClientInfoByName(const std::string name) const;
	void setHKDClientInfoWithName(
		const std::string name, 
		const std::string value);
	void processComponentMessage(DataPacketPtr pkt);
	void processDeviceMessage(
		const std::string fromID, 
		const std::string toID,
		DataPacketPtr pkt);
	void processEventMessage(
		const std::string fromID,
		const std::string toID,
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
		const std::string toID,
		const int command,
		const int result,
		const long long sequence,
		const std::vector<AbstractCamera>& cameras,
		const std::string did);

private:
	ASIOService asioService;
	AbstractDeviceGroup deviceGroup;
	HKComponentStreamSessionGroup streamSessionGroup;
	Vector<std::string> urlGroup;
	MajordomoWorkerPtr worker;
	std::string parentXMQID;
};//class HKDComponentClient

