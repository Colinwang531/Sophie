#include "boost/bind.hpp"
#include "boost/format.hpp"
#include "liburl/url.h"
using UrlParser = framework::liburl::UrlParser;
using ParamItem = framework::liburl::ParamItem;
#include "libcommon/uuid/uuid.h"
using Uuid = framework::libcommon::Uuid;
#include "libcommon/thread/threadpool.h"
using ThreadPool = framework::libcommon::ThreadPool;
#include "libcommon/time/time.h"
using Time = framework::libcommon::Time;
#include "libcommon/defs.h"
#include "libcommon/error.h"
#include "libprotocol/register_query_parser.h"
using RegisterQueryParser = framework::libprotocol::RegisterQueryParser;
#include "libprotocol/notify_parser.h"
using NotifyParser = framework::libprotocol::NotifyParser;
#include "libprotocol/alarm_parser.h"
using AlarmParser = framework::libprotocol::AlarmParser;
#include "alarm.h"

Alarm::Alarm(
	Log& log, 
	const std::string appID, 
	void* ctx /*= nullptr*/) 
	: Publisher(ctx), Worker(ctx), logObj{log}, clockTimetick{0},
	sourceID{Uuid().generateRandomUuid()}, applicationID{appID}, tid{nullptr}, stopped{false}
{}
Alarm::~Alarm()
{}

int Alarm::start(
	const std::string localIP,
	const unsigned short localPort,
	const std::string remoteIP,
	const unsigned short remotePort)
{
	CommonError e{
		static_cast<CommonError>(
			Publisher::bind(localIP, localPort))};

	if (CommonError::COMMON_ERROR_SUCCESS == e)
	{
		logObj.write(
			framework::liblog::LogLevel::LOG_LEVEL_INFO, 
			"Bind local IP [ %s ] and Port [ %d ] successfully.", 
			localIP.c_str(), 
			localPort);

		e = static_cast<CommonError>(
			Worker::connect(sourceID, remoteIP, remotePort));

		if (CommonError::COMMON_ERROR_SUCCESS == e)
		{
			logObj.write(
				framework::liblog::LogLevel::LOG_LEVEL_INFO, 
				"Connect remote IP [ %s ] and Port [ %d ] with ID [ %s ] successfully.", 
				remoteIP.c_str(), 
				remotePort,
				sourceID.c_str());
				
			tid = ThreadPool().create(
				boost::bind(&Alarm::timerTaskThreadHandler, this));
		}
		else
		{
			logObj.write(
				framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
				"Connect remote IP [ %s ] and Port [ %d ] with ID [ %s ] failed, reuslt = [ %d ].", 
				remoteIP.c_str(), 
				remotePort, 
				sourceID.c_str(), 
				static_cast<int>(e));
		}
	}
	else
	{
		logObj.write(
			framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
			"Bind local IP [ %s ] and Port [ %d ] for listening failed, reuslt = [ %d ].", 
			localIP.c_str(), 
			localPort, 
			static_cast<int>(e));
	}

	return static_cast<int>(e);
}

int Alarm::stop()
{
	stopped = true;
	ThreadPool().destroy(tid);
	tid = nullptr;
	return static_cast<int>(CommonError::COMMON_ERROR_SUCCESS);
}

void Alarm::afterWorkerPollDataProcess(const std::string data)
{
	if (!data.empty())
	{
		UrlParser urlParser;
		int e{urlParser.parse(data)};

		if (CommonError::COMMON_ERROR_SUCCESS == static_cast<CommonError>(e))
		{
			const std::string command{urlParser.getCommand()};
			
			if (0 == command.compare("register"))
			{
				processRegisterMessage(&urlParser);
			}
			else if (0 == command.compare("notify"))
			{
				processNotifyMessage(&urlParser);
			}
			else if (0 == command.compare("alarm"))
			{
				processAlarmMessage(&urlParser);
			}
			else
			{
				logObj.write(
					framework::liblog::LogLevel::LOG_LEVEL_WARNING, 
					"Not support command type [ %s ].", 
					command.c_str());
			}
		}
		else
		{
			logObj.write(
				framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
				"Parse poll data failed, result = [ %d ].", 
				e);
		}
	}
}

void Alarm::timerTaskThreadHandler()
{
	unsigned long long lastTick{0};

	while (!stopped)
	{
		unsigned long long nowTick{Time().tickcount()};

		if (30000 < nowTick - lastTick)
		{
			sendRegisterMessage();
			lastTick = nowTick;
		}
	}
}

void Alarm::sendRegisterMessage()
{
	//构建注册消息
	RegisterQueryParser registerParser;
	registerParser.setCommandType(RegisterQueryType::REGISTER_QUERY_TYPE_REGISTER_REQ);
	ApplicationInfo info;
	info.type = ApplicationType::APPLICATION_TYPE_ALARM;
	info.name = "Alarm";
	info.ID = applicationID;
	info.parentID = sourceID;
	registerParser.setRegisterApplicationInfo(info);
	const std::string registerData{registerParser.compose()};

	//发送"register://sender=sourceID & data=X"
	UrlParser urlParser;
	urlParser.setCommand("register");
	urlParser.setCommandParameter("sender", sourceID);
	urlParser.setCommandParameter("data", registerData);
	const std::string urlData{urlParser.compose()};
	int e{Worker::send(urlData)};

	if (CommonError::COMMON_ERROR_SUCCESS == static_cast<CommonError>(e))
	{
		logObj.write(
			framework::liblog::LogLevel::LOG_LEVEL_INFO, 
			"Send message [ %s ] successfully.", 
			urlData.c_str());
	}
	else
	{
		logObj.write(
			framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
			"Send message [ %s ] failed, result = [ %d ].", 
			urlData.c_str(),
			e);
	}
}

void Alarm::processRegisterMessage(void* parser /*= nullptr*/)
{
	UrlParser* urlparser{reinterpret_cast<UrlParser*>(parser)};
	const std::vector<ParamItem> commandParamItems{urlparser->getCommandParameters()};
	std::string data;

	for (int i = 0; i != commandParamItems.size(); ++i)
	{
		if (0 == commandParamItems[i].key.compare("data"))
		{
			data = commandParamItems[i].value;
			break;
		}
	}
	
	if (!data.empty())
	{
		RegisterQueryParser registerParser;
		CommonError e{
			static_cast<CommonError>(registerParser.parse(data))};
		RegisterQueryType type{registerParser.getCommandType()};

		if (CommonError::COMMON_ERROR_SUCCESS == e)
		{
			if (RegisterQueryType::REGISTER_QUERY_TYPE_REGISTER_REP == type)
			{
				const int statusCode{registerParser.getStatusCode()};
				if (CommonError::COMMON_ERROR_SUCCESS == static_cast<CommonError>(statusCode))
				{
					logObj.write(
						framework::liblog::LogLevel::LOG_LEVEL_INFO, 
						"Receive message of registration with status code = [ %d ].");
				}
				else
				{
					logObj.write(
						framework::liblog::LogLevel::LOG_LEVEL_WARNING, 
						"Receive message of registration with status code = [ %d ].",
						statusCode);
				}
			}
			else
			{
				logObj.write(
					framework::liblog::LogLevel::LOG_LEVEL_WARNING, 
					"Got unknown command type = [ %d ] of message.", 
					static_cast<int>(type));
			}
		}
		else
		{
			logObj.write(
				framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
				"Parse received message [ %s ] failed, result = [ %d ].", 
				data.c_str(),
				static_cast<int>(e));
		}
	}
	else
	{
		logObj.write(
			framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
			"Process received message without data.");
	}
}

void Alarm::processNotifyMessage(void* parser /*= nullptr*/)
{
	UrlParser* urlParser{reinterpret_cast<UrlParser*>(parser)};
	const std::vector<ParamItem> commandParamItems{urlParser->getCommandParameters()};
	std::string data;

	for (int i = 0; i != commandParamItems.size(); ++i)
	{
		if (0 == commandParamItems[i].key.compare("data"))
		{
			data = commandParamItems[i].value;
			break;
		}
	}
	
	if (!data.empty())
	{
		NotifyParser notifyParser;
		CommonError e{
			static_cast<CommonError>(notifyParser.parse(data))};
		NotifyType type{notifyParser.getNotifyType()};

		if (CommonError::COMMON_ERROR_SUCCESS == e)
		{
			if (NotifyType::NOTIFY_TYPE_CLOCK_SYC == type)
			{
				clockInfo = notifyParser.getClockInfo();
				if (0 == (clockTimetick++ % 300))
				{
					logObj.write(
						framework::liblog::LogLevel::LOG_LEVEL_INFO, 
						"Receive notification message of clock time [ %s ].",
						clockInfo.c_str());
				}
			}
			else
			{
				logObj.write(
					framework::liblog::LogLevel::LOG_LEVEL_WARNING, 
					"Not support notification message type = [ %d ].", 
					static_cast<int>(type));
			}
		}
		else
		{
			logObj.write(
				framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
				"Parse notification message [ %s ] failed, result = [ %d ].", 
				data.c_str(),
				static_cast<int>(e));
		}
	}
	else
	{
		logObj.write(
			framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
			"Process received message without data.");
	}
}

void Alarm::processAlarmMessage(void* parser /*= nullptr*/)
{
	UrlParser* urlparser{reinterpret_cast<UrlParser*>(parser)};
	const std::vector<ParamItem> commandParamItems{urlparser->getCommandParameters()};
	std::string data;

	for (int i = 0; i != commandParamItems.size(); ++i)
	{
		if (0 == commandParamItems[i].key.compare("data"))
		{
			data = commandParamItems[i].value;
			break;
		}
	}
	
	if (!data.empty())
	{
		AlarmParser alarmParser;
		CommonError e{
			static_cast<CommonError>(alarmParser.parse(data))};

		if (CommonError::COMMON_ERROR_SUCCESS == e)
		{
			AlarmInfo info{alarmParser.getAlarmInfo()};
			//用母钟时间戳更新报警数据的时间戳
			info.timestamp = clockInfo;
			alarmParser.setAlarmInfo(info);
			const std::string newData{alarmParser.compose()};

			if (!newData.empty())
			{
				int e{Publisher::send(newData)};

				if (CommonError::COMMON_ERROR_SUCCESS == static_cast<CommonError>(e))
				{
					logObj.write(
						framework::liblog::LogLevel::LOG_LEVEL_INFO, 
						"Send alarm message of publisher successfully.");
				}
				else
				{
					logObj.write(
						framework::liblog::LogLevel::LOG_LEVEL_WARNING, 
						"Send alarm message of publisher [ %s ] failed, result = [ %d ].",
						newData.c_str(),
						e);
				}
			}
			else
			{
				logObj.write(
					framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
					"Compose alarm message with clock information failed.");
			}
		}
		else
		{
			logObj.write(
				framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
				"Parse alarm message [ %s ] failed, result = [ %d ].", 
				data.c_str(),
				static_cast<int>(e));
		}
	}
	else
	{
		logObj.write(
			framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
			"Process received message without data.");
	}
}
