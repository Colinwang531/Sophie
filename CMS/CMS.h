//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-12-18
//		Description :					资源管理应用类
//
//		History:						Author									Date										Description
//										王科威									 2020-12-18									 创建
//

#ifndef CMS_H
#define CMS_H

#include "libnetwork/zmq/module/worker.h"
using Worker = framework::libnetwork::zmq::module::Worker;
#include "liblog/log.h"
using Log = framework::liblog::Log;

class CMS final : protected Worker
{
public:
	CMS(
		Log& log, 
		const std::string appID,
		void* ctx = nullptr);
	~CMS(void);

public:
	//连接
	//@remoteIP : 远程服务IP
	//@remotePort : 远程服务端口
	//@Return : 错误码
	int connect(
		const std::string remoteIP,
		const unsigned short remotePort);

	//建立
	//@uploadIP : 注册服务IP
	//@uploadPort : 注册服务端口
	//@Return : 错误码
	int setup(
		const std::string uploadIP,
		const unsigned short uploadPort);

protected:
	void afterWorkerPollDataProcess(
		const std::string data) override;

private:
	void timerTaskHandler(void);
	void processPairMessage(void* parser = nullptr);
	void processRegisterMessage(void* parser = nullptr);
	void sendPairMessage(void);
	void sendRegisterMessage(const std::string viaID);

private:
	Log& logObj;
	const std::string applicationID;
	const std::string sourceID;
	void* tid;
	bool stopped;
};//class CMS

#endif//CMS_H
