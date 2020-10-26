//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-06-15
//		Description :					�����豸����ͻ�����
//
//		History:						Author									Date										Description
//										������									2020-05-13									����
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
	//���ӷ����
	//@did : �豸ID��ʶ
	//@cid : �����ID��ʶ
	//@idx : ���������
	//@Return : ������
	int connectMediaServer(
		const std::string did,
		const std::string cid,
		const int idx = -1);

	//�Ͽ����������
	//@did : �豸ID��ʶ
	//@Return : ������
	//@Comment : �Ͽ��豸�����е�����
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

