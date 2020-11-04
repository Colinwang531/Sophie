//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-03
//		Description :					������������ͻ�����
//
//		History:						Author									Date										Description
//										������									2020-10-03									����
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
	//WORKER�����ݶ�ȡ�ص�
	//@roleID : ��ɫID��ʶ
	//@flagID : ��־ID��ʶ
	//@fromID : ������ID��ʶ
	//@toID : ������ID��ʶ
	//@data : ��Ϣ����
	//@Return : ������
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

