//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-12
//		Description :					XMQ代理
//
//		History:						Author									Date										Description
//										王科威									 2021-06-12									 创建
//

#ifndef XMQ_H
#define XMQ_H

#include "libnetwork/zmq/module/dispatcher.h"
using Dispatcher = framework::network::zmq::module::Dispatcher;
#include "liblog/log.h"
using Log = framework::liblog::Log;

class XMQ final : public Dispatcher
{
public:
	XMQ(Log& log_);
	XMQ(
		Log& log_,
		const std::string localIP,
		const unsigned short localPort,
		const std::string uplinkIP,
		const unsigned short uplinkPort);
	~XMQ(void);

public:
	int start(void) override;
	int stop(void* = nullptr) override;

protected:
	void afterRouterPollDataProcess(
		const std::string sender,
		const std::string data) override;
	void afterDealerPollDataProcess(
		const std::string data) override;

private:
	//保存或更新与XMQ相匹配的CMS ID标识
	//@id : CMS ID标识
	//@Return : 错误码
	int updatePairedCMSID(const std::string id);
/*
private:
	//������Ϣ
	void processPolledMessage(
		const std::string sender,
		const std::string module,
		const std::string from,
		const std::string to, 
		const std::vector<std::string> messages);

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
*/
private:
	std::string pairedCMSID;
	const std::string bindIP;
	const unsigned short bindPort;
	const std::string connectIP;
	const unsigned short connectPort;
	void* ctx;
	Log& log;
};//class XMQ

#endif//XMQ_H
