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
#include "libprotocol/defs.h"
using ApplicationInfo = framework::libprotocol::ApplicationInfo;
#include "libcommon/data_struct/mutable_array/mutable_array.h"

class CMS final : protected Worker
{
public:
	CMS(
		Log& log, 
		const std::string appID,
		void* ctx = nullptr);
	~CMS(void);

public:
	//启动
	//@remoteIP : 远程服务IP
	//@remotePort : 远程服务端口
	//@uploadIP : 注册服务IP
	//@uploadPort : 注册服务端口
	//@Return : 错误码
	int start(
		const std::string remoteIP,
		const unsigned short remotePort,
		const std::string uploadIP,
		const unsigned short uploadPort);

	//停止
	int stop(void);

protected:
	void afterWorkerPollDataProcess(const std::string data) override;

private:
	void timerTaskThreadHandler(void);
	void sendRegisterMessage(void);
	void sendRegisterReplyMessage(
		const std::string receiver, 
		const std::string via,
		const bool isVia = false);
	void sendQueryReplyMessage(
		const std::string receiver, 
		const std::string via,
		const bool isVia = false);
	void processRegisterQueryMessage(void* parser = nullptr);
	void processRegisterQueryMessage(
		const std::string sender, 
		const std::string data,
		const std::string target,
		const bool isVia = false);
	void removeExpiredApplicationInfo(void);
	int updateRegisterApplicationInfo(
		const std::string sender,
		const std::string via, 
		const bool isVia = false, 
		void* parser = nullptr);

private:
	Log& logObj;
	const std::string applicationID;
	const std::string sourceID;
	std::string viaID;
	std::string uploadIP;
	unsigned short uploadPort;
	void* tid;
	bool stopped;
	MutableArray<ApplicationInfo> applicationInfos;
};//class CMS

#endif//CMS_H
