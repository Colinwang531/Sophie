//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-03
//		Description :					报警推送组件客户端类
//
//		History:						Author									Date										Description
//										王科威									2020-10-03									创建
//

#include "boost/shared_ptr.hpp"
#include "Packet/DataPacket.h"
using DataPacket = base::packet::DataPacket;
using DataPacketPtr = boost::shared_ptr<DataPacket>;
#include "MessageQueue/MajordomoWorker.h"
using MajordomoWorker = mq::module::MajordomoWorker;
using MajordomoWorkerPtr = boost::shared_ptr<MajordomoWorker>;
#include "Network/AbstractWorker.h"
using AbstractWorker = base::network::AbstractWorker;

class AlarmPusherComponentClient : public AbstractWorker
{
public:
	AlarmPusherComponentClient(void);
	virtual ~AlarmPusherComponentClient(void);

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
	void sendQuerySystemServiceMessage(void) override;
	int sendData(
		const std::string roleID,
		const std::string flagID,
		const std::string fromID,
		const std::string toID,
		const std::string data) override;

private:
	const std::string getMediaStreamClientInfoByName(const std::string name) const;
	void setMediaStreamClientInfoWithName(
		const std::string name, 
		const std::string value);
	void processComponentMessage(DataPacketPtr pkt);
	void processAlarmMessage(DataPacketPtr pkt);
	void processEventMessage(DataPacketPtr pkt);

private:
	MajordomoWorkerPtr worker;
	std::string parentXMQID;
	std::string webComponentID;
	std::string motherClockTime;
};//class AlarmPusherComponentClient

