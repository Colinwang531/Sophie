//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-13
//		Description :					��Ϣ�ַ��첽�������
//
//		History:						Author									Date										Description
//										������									2020-05-13									����
//

#include "boost/shared_ptr.hpp"
#include "DataStruct/UnorderedMap.h"
#include "Network/AbstractUpstreamServer.h"
using AbstractUpstreamServer = base::network::AbstractUpstreamServer;
using ServerModuleType = base::network::ServerModuleType;
using ClientModuleType = base::network::ClientModuleType;
#include "Packet/DataPacket.h"
using DataPacket = base::packet::DataPacket;
using DataPacketPtr = boost::shared_ptr<DataPacket>;
#include "Component/AbstractComponent.h"
using AbstractComponent = base::component::AbstractComponent;
using AbstractComponentPtr = boost::shared_ptr<AbstractComponent>;

class MDCServer final : public AbstractUpstreamServer
{
public:
	MDCServer(
		const ServerModuleType server = ServerModuleType::SERVER_MODULE_TYPE_NONE,
		const ClientModuleType upstream = ClientModuleType::CLIENT_MODULE_TYPE_NONE,
		const std::string address = "tcp:\\127.0.0.1:61001",
		const std::string name = "MDCServer");
	~MDCServer(void);

protected:
	void afterServerPolledMessageProcess(
		const std::string commID,
		const std::string flagID,
		const std::string fromID,
		const std::string toID,
		const std::string msg) override;
	void afterClientPolledMessageProcess(
		const std::string flagID,
		const std::string fromID,
		const std::string toID,
		const std::string msg) override;
	void afterAutoCheckConnectionTimeoutProcess(void) override;
	const std::string buildAutoRegisterToBrokerMessage(void) override;

private:
	const std::string getMDCServerInfoByName(
		const std::string name) const;
	void setMDCServerInfoWithName(
		const std::string name, const std::string value);

	//��������ݽ��մ���
	//@commID : ͨ��ID��ʶ
	//@flagID : Request/Response��ʶ
	//@fromID : ������ID��ʶ
	//@toID : ������ID��ʶ
	//@msg : ��Ϣ����
	void processServerPolledMessage(
		const std::string commID,
		const std::string flagID,
		const std::string fromID,
		const std::string toID,
		const std::string msg);
	//���������ת������
	//@commID : ͨ��ID��ʶ
	//@flagID : Request/Response��ʶ
	//@fromID : ������ID��ʶ
	//@toID : ������ID��ʶ
	//@msg : ��Ϣ����
	void forwardServerPolledMessage(
		const std::string commID,
		const std::string flagID,
		const std::string fromID,
		const std::string toID,
		const std::string msg);

	//���������Ϣ
	//@commID : ͨ��ID��ʶ
	//@flagID : Request/Response��ʶ
	//@fromID : ������ID��ʶ
	//@pkt : ��Ϣ����
	void processComponentMessage(
		const std::string commID, 
		const std::string flagID,
		const std::string fromID,
		DataPacketPtr pkt);

	//ת��״̬������Ϣ
	//@fwdmsg : ��Ϣ����
	//@commID : ͨ��ID��ʶ
	//@status : ������Ϣ
	//@sequence : ������Ϣ���к�
// 	void forwardStatusConfigureMessage(
// 		const std::string fwdmsg,
// 		const std::string commID,
// 		void* status = nullptr,
// 		const long long sequence = -1);

	//ת���豸��Ϣ
	//@commID : ͨ��ID��ʶ
	//@pkt : ������Ϣ
	//@msg : ��Ϣ����
// 	void forwardDeviceMessage(
// 		const std::string commID,
// 		DataPacketPtr pkt,
// 		const std::string msg);

	//ת���㷨��Ϣ
	//@fwdmsg : ��Ϣ����
	//@commID : ͨ��ID��ʶ
	//@algorithm : ������Ϣ
	//@sequence : ������Ϣ���к�
// 	void forwardAlgorithmConfigureMessage(
// 		const std::string fwdmsg,
// 		const std::string commID,
// 		void* algorithm = nullptr,
// 		const long long sequence = -1);

	//ת������������Ϣ
	//@s : SOCKET��ʶ
	//@data : ���������ַ���
	//@databytes : ���������ֽ���
// 	void forwardAlarmInfoMessage(
// 		void* s = nullptr,
// 		const void* data = nullptr,
// 		const unsigned int databytes = 0);

	//ת����Ա��Ϣ
	//@fwdmsg : ��Ϣ����
	//@commID : ͨ��ID��ʶ
	//@crew : ������Ϣ
	//@sequence : ������Ϣ���к�
// 	void forwardCrewConfigureMessage(
// 		const std::string fwdmsg,
// 		const std::string commID,
// 		void* crew = nullptr,
// 		const long long sequence = -1);

	//�����ע������
	//@componentID : ���ID��ʶ
	//@serviceName : �����������
	//@communicationID : ͨ��ID��ʶ
	//@componentType : �������
	//@Return : ������
	//@Comment : ͨ��replyAddNewRegisterComponent����Ӧ��
	int addNewRegisterComponent(
		const std::string componentID,
		const std::string serviceName,
		const std::string communicationID,
		const base::component::ComponentType componentType = base::component::ComponentType::COMPONENT_TYPE_NONE);

	//���������Ϣ
	//@componentID : ���ID��ʶ
	//@serviceName : �������
	//@communicationID : ͨ��ID��ʶ
	//@componentType : �������
	//@Return : ������
	//@Comment : ��Ӧ��
	int updateRegisterCompnent(
		const std::string componentID,
		const std::string serviceName,
		const std::string communicationID,
		const base::component::ComponentType componentType = base::component::ComponentType::COMPONENT_TYPE_NONE);

	//ɾ��ע�����
	//@componentID : ���ID��ʶ
	//@componentType : �������
	//@Return : ������
	//@Comment : ��Ӧ��
	int removeRegisterComponent(
		const std::string componentID,
		const base::component::ComponentType componentType = base::component::ComponentType::COMPONENT_TYPE_NONE);

	//�����Ӧ����������Ϣ
	//@cid : ���ID��ʶ
	//@result : ע��״̬
	//@sequence : ��Ϣ���к�
	//@s : SOCKET��ʶ
	//@commID : ͨ��ID��ʶ
	//@idbytes : ID��ʶ�ֽ���
	//@Return : ������
	int replyMessageWithResultAndExtendID(
		const std::string commID,
		const std::string extendID,
		const std::string fromID,
		const int pkttype = 0,
		const int command = 0,
		const int result = 0,
		const long long sequence = 0);

	//��ָ�������ת����Ϣ
	//@fwdmsg : ��Ϣ����
	//@componentType : �������
	//@Return : ������
// 	int forwardMessageByComponentType(
// 		const std::string fwdmsg,
// 		const base::component::ComponentType componentType = base::component::ComponentType::COMPONENT_TYPE_NONE);

private:
	//ע���������
	//key��ͨ��ID��ʶ
	using RegisterComponentGroup = UnorderedMap<const std::string, AbstractComponentPtr>;
	RegisterComponentGroup registerComponentGroup;
};//class MDCServer

