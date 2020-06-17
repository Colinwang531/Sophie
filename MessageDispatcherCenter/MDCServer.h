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

#include "DataStruct/UnorderedMap.h"
#include "MessageQueue/AsynchronousUpstreamServer.h"
using AsynchronousUpstreamServer = mq::module::AsynchronousUpstreamServer;

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
		const char* id = nullptr, 
		const unsigned int idbytes = 0,
		const char* delimiter = nullptr, 
		const unsigned int delimiterbytes = 0,
		const char* data = nullptr, 
		const unsigned int databytes = 0) override;

	//���η�������ݴ���
	//@msg : ��������
	//@bytes : �����ֽ���
	void afterUpstreamPollMessage(
		const char* msg = nullptr, 
		const unsigned int bytes = 0) override;

private:
	//ע������������¼���߳�
	void updateComponentCheckingThreadProc(void);

	//�����������Ϣ
	//@s : SOCKET��ʶ
	//@mid : ��ϢID��ʶ
	//@idbytes : ID��ʶ�ֽ���
	//@pkt : ��Ϣ��
	void dealWithComponentMessage(
		void* s = nullptr,
		const char* mid = nullptr,
		const unsigned int idbytes = 0,
		void* pkt = nullptr);

	//����״̬����Ϣ
	//@s : SOCKET��ʶ
	//@mid : ��ϢID��ʶ
	//@idbytes : ID��ʶ�ֽ���
	//@pkt : ��Ϣ��
	void dealWithStatusMessage(
		void* s = nullptr,
		const char* mid = nullptr,
		const unsigned int idbytes = 0,
		void* pkt = nullptr);

	//��������Ϣ
	//@cname : �������
	//@type : �������
	//@Return : ���UUID��ʶ
	//			����ֵ�ձ�ʾ���ʧ��
	const std::string addComponentInfo(
		const std::string cname,
		const int type = 0);

	//Ӧ����������Ϣ
	//@cid : ���ID��ʶ
	//@result : ע��״̬
	//@sequence : ��Ϣ���к�
	//@s : SOCKET��ʶ
	//@mid : ��ϢID��ʶ
	//@idbytes : ID��ʶ�ֽ���
	//@Return : ������
	int replyAddComponentInfo(
		const std::string cid,
		const int result = 0,
		const long long sequence = 0,
		void* s = nullptr,
		const char* mid = nullptr,
		const unsigned int idbytes = 0);

	//ɾ��ע�����
	//@cid : ���ID��ʶ
	//@type : �������
	//@Return : ������
	//@Comment : ��Ӧ��
	int removeComponentInfo(
		const std::string cid,
		const int type = 0);

	//�������������Ϣ
	//@cid : ���ID��ʶ
	//@type : �������
	//@Return : ������
	//@Comment : ��Ӧ��
	int updateCompnentInfo(
		const std::string cid,
		const int type = 0);

	//Ӧ���ѯ�����Ϣ
	//@rcis : �����Ϣ����
	//@result : ע��״̬
	//@sequence : ��Ϣ���к�
	//@s : SOCKET��ʶ
	//@mid : ��ϢID��ʶ
	//@idbytes : ID��ʶ�ֽ���
	//@Return : ������
	int replyQueryComponentInfo(
		const std::vector<void*>& rcis,
		const int result = 0,
		const long long sequence = 0,
		void* s = nullptr,
		const char* mid = nullptr,
		const unsigned int idbytes = 0);

	//����Ӧ����Ϣ
	//@s : SOCKET��ʶ
	//@mid : ��ϢID��ʶ
	//@idbytes : ID��ʶ�ֽ���
	//@msg : ��Ϣ�ַ���
	//@bytes : ��Ϣ�ֽ���
	//@Return : ������
	int sendReplyMessage(
		void* s = nullptr, 
		const char* mid = nullptr,
		const unsigned int idbytes = 0,
		const char* msg = nullptr,
		const unsigned int bytes = 0);

private:
	//ע���������
	using RegisterComponentInfos = UnorderedMap<const std::string, void*>;
	RegisterComponentInfos registerComponentInfos;
	//�÷������ڽڵ�����ʱ����3�����Բ���Ч
	//��3�����Է��������Եò��ǺܺϺ��߼�
	//true��ʾ����,false��ʾͣ��,Ĭ��ֵΪtrue
	bool sailStatus;
	//true��ʾ�Զ�,false��ʾ�ֶ�,Ĭ��ֵΪtrue
	bool autoStatus;
	std::string nodeName;
};//class MDCServer

