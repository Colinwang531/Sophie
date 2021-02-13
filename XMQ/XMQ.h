//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-13
//		Description :					XMQ��
//
//		History:						Author									Date										Description
//										������									2020-05-13									����
//										������									2020-11-25									1.�ع���Ϣ�ַ�����������
//										     									          									2.�޸�CMS������Ӽ���߳�Ϊҵ���߼��߳�
//

#ifndef XMQ_H
#define XMQ_H

//#include "boost/shared_ptr.hpp"
// #include "MessageQueue/MQMessagePacket.h"
// using MQMessagePacket = mq::message::MQMessagePacket;
// #include "MessageQueue/MajordomoBroker.h"
// using MajordomoBroker = mq::module::MajordomoBroker;
// using MajordomoBrokerPtr = boost::shared_ptr<MajordomoBroker>;
// #include "MessageQueue/MajordomoWorker.h"
// using MajordomoWorker = mq::module::MajordomoWorker;
// using MajordomoWorkerPtr = boost::shared_ptr<MajordomoWorker>;
#include "Network/ZMQ/RD.h"
using RD = framework::network::zeromq::RD;
// #include "Network/AbstractServer.h"
// using AbstractServer = base::network::AbstractServer;
// #include "Network/AbstractWorker.h"
// using AbstractWorker = base::network::AbstractWorker;

class XMQ final : public RD
{
public:
	XMQ(void);
	~XMQ(void);

public:
	int startRD(
		const unsigned short rlp = 0,
		const char* dcip = nullptr,
		const unsigned short dcp = 0) override;
	int stopRD(void) override;

protected:
	void afterParsePolledMessage(
		const std::string sender,
		const std::string module,
		const std::string from,
		const std::string to,
		const std::vector<std::string> routers,
		const std::vector<std::string> messages) override;
	void afterParsePolledMessage(
		const std::string module,
		const std::string from,
		const std::string to,
		const std::vector<std::string> routers,
		const std::vector<std::string> messages) override;

private:
	//������Ϣ
	void processPolledMessage(
		const std::string sender,
		const std::string module,
		const std::string from,
		const std::string to, 
		const std::vector<std::string> messages);

	//����CMS�����Ϣ
	//@commID : ������ID��ʶ
	//@module : ������ģ�ͱ�ʶ
	//@from : ����������
	//@to : ����������
	//@Comment : Ӧ�����״̬
	void processCMSPairMessage(
		const std::string sender,
		const std::string module,
		const std::string from,
		const std::string to,
		const std::string sequence,
		const std::string timestamp);

	//ת����Ϣ
	//@receiver : ������ID��ʶ
	//@receiver : ������ID��ʶ
	//@module : ģ�ͱ�ʶ
	//@from : ����������
	//@to : ����������
	//@messages : ��Ϣ����
	void forwardPolledMessage(
		const std::string receiver,
		const std::string sender,
		const std::string module,
		const std::string from,
		const std::string to,
		const std::vector<std::string> messages);

private:
	std::string cmsCommID;
};//class XMQ

#endif//XMQ_H
