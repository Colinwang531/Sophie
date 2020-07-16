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
#include "MessageQueue/AsynchronousUpstreamServer.h"
using AsynchronousUpstreamServer = mq::module::AsynchronousUpstreamServer;
#include "Component/AbstractComponent.h"
using AbstractComponent = base::component::AbstractComponent;
using AbstractComponentPtr = boost::shared_ptr<AbstractComponent>;

class MDCServer : public AsynchronousUpstreamServer
{
public:
	MDCServer(void);
	~MDCServer(void);

protected:
	//�������ݶ�ȡ�߳�
	//@Return : ������
	int startPoller(void) override;

	//���ط�������ݴ���
	//@s : SOCKET��ʶ
	//@id : ID��ʶ�ַ���
	//@idbytes : ID��ʶ�ֽ���
	//@delimiter : �ָ����ַ���
	//@delimiterbytes : �ָ����ֽ���
	//@data : ���������ַ���
	//@databytes : ���������ֽ���
	//@Comment : ���ݴ����ʵ�ֱ��������������,���಻�����κ�����
	void afterServerPollMessage(
		void* s = nullptr,
		const void* id = nullptr, 
		const unsigned int idbytes = 0,
		const void* delimiter = nullptr, 
		const unsigned int delimiterbytes = 0,
		const void* data = nullptr, 
		const unsigned int databytes = 0) override;

	//���η�������ݴ���
	//@msg : ��������
	//@bytes : �����ֽ���
	void afterUpstreamPollMessage(
		const void* msg = nullptr, 
		const unsigned int bytes = 0) override;

private:
	//�Զ�����������״̬�߳�
	void autoCheckOfflinStatusOfComponentThreadProc(void);

	//�����������Ϣ
	//@s : SOCKET��ʶ
	//@commID : ͨ��ID��ʶ
	//@idbytes : ID��ʶ�ֽ���
	//@pkt : ��Ϣ��
	void dealWithComponentMessage(
		void* s = nullptr,
		const void* commID = nullptr,
		const unsigned int idbytes = 0,
		void* pkt = nullptr);

	//ת��״̬������Ϣ
	//@s : SOCKET��ʶ
	//@commID : ͨ��ID��ʶ
	//@idbytes : ID��ʶ�ֽ���
	//@pkt : ��Ϣ��
	//@data : ���������ַ���
	//@databytes : ���������ֽ���
	void forwardStatusConfigureMessage(
		void* s = nullptr,
		const void* commID = nullptr,
		const unsigned int idbytes = 0,
		void* pkt = nullptr,
		const void* data = nullptr,
		const unsigned int databytes = 0);

	//ת���豸������Ϣ
	//@s : SOCKET��ʶ
	//@commID : ͨ��ID��ʶ
	//@idbytes : ID��ʶ�ֽ���
	//@pkt : ��Ϣ��
	//@data : ���������ַ���
	//@databytes : ���������ֽ���
	void forwardDeviceConfigureMessage(
		void* s = nullptr,
		const void* commID = nullptr,
		const unsigned int idbytes = 0,
		void* pkt = nullptr,
		const void* data = nullptr,
		const unsigned int databytes = 0);

	//ת���㷨������Ϣ
	//@s : SOCKET��ʶ
	//@commID : ͨ��ID��ʶ
	//@idbytes : ID��ʶ�ֽ���
	//@pkt : ��Ϣ��
	//@data : ���������ַ���
	//@databytes : ���������ֽ���
	void forwardAlgorithmConfigureMessage(
		void* s = nullptr,
		const void* commID = nullptr,
		const unsigned int idbytes = 0,
		void* pkt = nullptr,
		const void* data = nullptr,
		const unsigned int databytes = 0);

	//ת������������Ϣ
	//@s : SOCKET��ʶ
	//@data : ���������ַ���
	//@databytes : ���������ֽ���
	void forwardAlarmInfoMessage(
		void* s = nullptr,
		const void* data = nullptr,
		const unsigned int databytes = 0);

	//ת����Ա��Ϣ������Ϣ
	//@s : SOCKET��ʶ
	//@commID : ͨ��ID��ʶ
	//@idbytes : ID��ʶ�ֽ���
	//@pkt : ��Ϣ��
	//@data : ���������ַ���
	//@databytes : ���������ֽ���
	void forwardCrewConfigureMessage(
		void* s = nullptr,
		const void* commID = nullptr,
		const unsigned int idbytes = 0,
		void* pkt = nullptr,
		const void* data = nullptr,
		const unsigned int databytes = 0);

	//�����ע������
	//@c : ���ʵ��
	void addRegisterComponent(void* c = nullptr);

	//�����Ӧ����������Ϣ
	//@cid : ���ID��ʶ
	//@result : ע��״̬
	//@sequence : ��Ϣ���к�
	//@s : SOCKET��ʶ
	//@commID : ͨ��ID��ʶ
	//@idbytes : ID��ʶ�ֽ���
	//@Return : ������
	int replyAddRegisterComponent(
		const std::string cid,
		const int result = 0,
		const long long sequence = 0,
		void* s = nullptr,
		const void* commID = nullptr,
		const unsigned int idbytes = 0);

	//ɾ��ע�����
	//@cid : ���ID��ʶ
	//@type : �������
	//@Return : ������
	//@Comment : ��Ӧ��
	int removeRegisterComponent(
		const std::string cid,
		const int type = 0);

	//���������Ϣ
	//@c : ���ʵ��
	//@Return : ������
	//@Comment : ��Ӧ��
	int updateRegisterCompnent(void* c = nullptr);

	//Ӧ�������ѯ����
	//@componentPtrs : �������
	//@result : ע��״̬
	//@sequence : ��Ϣ���к�
	//@s : SOCKET��ʶ
	//@commID : ͨ��ID��ʶ
	//@idbytes : ID��ʶ�ֽ���
	//@Return : ������
	int replyQueryRegisterComponent(
		const std::vector<AbstractComponentPtr>& componentPtrs,
		const int result = 0,
		const long long sequence = 0,
		void* s = nullptr,
		const void* commID = nullptr,
		const unsigned int idbytes = 0);

	//�����Ӧ��������ʧ��������Ϣ
	//@command : ��������
	//@sequence : ��Ϣ���к�
	//@s : SOCKET��ʶ
	//@commID : ͨ��ID��ʶ
	//@idbytes : ID��ʶ�ֽ���
	//@Return : ������
	int replyConfigureSetFailedMessage(
		const int command = 0,
		const long long sequence = 0,
		void* s = nullptr,
		const void* commID = nullptr,
		const unsigned int idbytes = 0);

	//���ͷ����Ӧ����Ϣ
	//@s : SOCKET��ʶ
	//@commID : ͨ��ID��ʶ
	//@idbytes : ID��ʶ�ֽ���
	//@data : ��Ϣ�ַ���
	//@databytes : ��Ϣ�ֽ���
	//@Return : ������
	int sendServerResponseMessage(
		void* s = nullptr, 
		const void* commID = nullptr,
		const unsigned int idbytes = 0,
		const void* data = nullptr,
		const unsigned int databytes = 0);

	//ת���������������Ӧ����Ϣ
	//@s : SOCKET��ʶ
	//@type : Ŀ�Ķ��������
	//@data : ���������ַ���
	//@databytes : ���������ֽ���
	//@Return : ������
	int forwardConfigureRequestOrResponseMessage(
		void* s = nullptr, 
		const int type = 0, 
		const void* data = nullptr, 
		const unsigned int databytes = 0);

private:
	//ע���������
	using RegisterComponentGroup = UnorderedMap<const std::string, AbstractComponentPtr>;
	RegisterComponentGroup registerComponentGroup;
	//�÷������ڽڵ�����ʱ����3�����Բ���Ч
	//��3�����Է��������Եò��ǺܺϺ��߼�
	//true��ʾ����,false��ʾͣ��,Ĭ��ֵΪtrue
	bool sailStatus;
	//true��ʾ�Զ�,false��ʾ�ֶ�,Ĭ��ֵΪtrue
	bool autoStatus;
	std::string nodeName;
};//class MDCServer

