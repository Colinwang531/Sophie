//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-12
//		Description :					消息发布类
//
//		History:						Author									Date										Description
//										王科威									 2021-06-12									 创建
//

#ifndef ALARM_H
#define ALARM_H

#include "libnetwork/zmq/module/publisher.h"
using Publisher = framework::libnetwork::zmq::module::Publisher;
#include "libnetwork/zmq/module/worker.h"
using Worker = framework::libnetwork::zmq::module::Worker;
#include "liblog/log.h"
using Log = framework::liblog::Log;

class Alarm final : protected Publisher, protected Worker
{
public:
	Alarm(
		Log& log, 
		const std::string appID, 
		void* ctx = nullptr);
	~Alarm(void);

public:
	//启动
	//@remoteIP : 本地服务IP
	//@remotePort : 本地服务端口
	//@uploadIP : 远程服务IP
	//@uploadPort : 远程服务端口
	//@Return : 错误码
	int start(
		const std::string localIP,
		const unsigned short localPort,
		const std::string remoteIP,
		const unsigned short remotePort);

	//停止
	int stop(void);

protected:
	void afterWorkerPollDataProcess(const std::string data) override;

private:
	void timerTaskThreadHandler(void);
	void sendRegisterMessage(void);
	void processRegisterMessage(void* parser = nullptr);
	void processNotifyMessage(void* parser = nullptr);
	void processAlarmMessage(void* parser = nullptr);

private:
	Log& logObj;
	const std::string applicationID;
	std::string sourceID;
	std::string clockInfo;
	void* tid;
	bool stopped;
	unsigned long long clockTimetick;
};//class Alarm

#endif//ALARM_H

