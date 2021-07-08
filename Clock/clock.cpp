#include "boost/algorithm/string.hpp"
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
#include "clock.h"

Clock::Clock(
	Log& log, 
	const std::string appID, 
	void* ctx /*= nullptr*/) 
	: Worker(ctx), logObj{log}, applicationID{appID}, 
	sourceID{Uuid().generateRandomUuid()}, tid{nullptr}, stopped{false}
{}
Clock::~Clock()
{}

int Clock::start(
	const std::string remoteIP,
	const unsigned short remotePort,
	const int number /*= -1*/,
	const int baudrate /*= 4800*/)
{
	CommonError e{
		static_cast<CommonError>(
			Worker::connect(sourceID, remoteIP, remotePort))};

	if (CommonError::COMMON_ERROR_SUCCESS == e)
	{
		logObj.write(
			framework::liblog::LogLevel::LOG_LEVEL_INFO, 
			"Connect remote IP [ %s ] and Port [ %d ] with ID [ %s ] successfully.", 
			remoteIP.c_str(), 
			remotePort,
			sourceID.c_str());
		
		tid = ThreadPool().create(
			boost::bind(&Clock::timerTaskThreadHandler, this));
		e = static_cast<CommonError>(
			com.start(number, baudrate, boost::bind(&Clock::afterComReadDataHandler, this, _1, _2)));

		if (CommonError::COMMON_ERROR_SUCCESS == e)
		{
			logObj.write(
				framework::liblog::LogLevel::LOG_LEVEL_INFO, 
				"Open COM port [ %d ] and baudrate [ %d ] successfully.", 
				number, 
				baudrate);
		}
		else
		{
			logObj.write(
				framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
				"Open COM port [ %d ] and baudrate [ %d ] failed, result = [ %d ].", 
				number, 
				baudrate,
				static_cast<int>(e));
		}
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

	return static_cast<int>(e);
}

int Clock::stop()
{
	stopped = true;
	com.stop();
	ThreadPool().destroy(tid);
	tid = nullptr;
	return static_cast<int>(CommonError::COMMON_ERROR_SUCCESS);
}

void Clock::afterWorkerPollDataProcess(const std::string data)
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
			else if (0 == command.compare("query"))
			{
				processQueryMessage(&urlParser);
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

void Clock::afterComReadDataHandler(
	const char* data /* = nullptr */, 
	const int bytes /* = 0 */)
{
	if (data && 0 < bytes)
	{
		clockData.append(data, bytes);

		if ('\n' == *data)
		{
			if (!clockData.empty())
			{
				std::vector<std::string> items;
				boost::split(items, clockData, boost::is_any_of(","));

				if (!items[0].compare("$ZQZDA") || !items[0].compare("$CJZDA") || !items[0].compare("$GPZDA"))
				{
					char clockDesc[128]{ 0 };
#ifdef WINDOWS
					sprintf_s(clockDesc, 128, "%s-%s-%s,%s:%s:%s,%s",
						items[4].c_str(), items[3].c_str(), items[2].c_str(),
						items[1].substr(0, 2).c_str(), items[1].substr(2, 2).c_str(), items[1].substr(4, 2).c_str(),
						items[5].c_str());
#else
					sprintf(clockDesc, "%s-%s-%s,%s:%s:%s,%s",
						items[4].c_str(), items[3].c_str(), items[2].c_str(),
						items[1].substr(0, 2).c_str(), items[1].substr(2, 2).c_str(), items[1].substr(4, 2).c_str(),
						items[5].c_str());
#endif//WINDOWS

					sendClockMessage(clockDesc);
				}

				clockData.clear();
			}
		}
	}
}

void Clock::timerTaskThreadHandler()
{
	unsigned long long lastTick{0};

	while (!stopped)
	{
		unsigned long long nowTick{Time().tickcount()};

		if (30000 < nowTick - lastTick)
		{
			sendRegisterMessage();
			sendQueryMessage();
			lastTick = nowTick;
		}
	}
}

void Clock::sendRegisterMessage()
{
	//构建注册消息
	RegisterQueryParser registerParser;
	registerParser.setCommandType(RegisterQueryType::REGISTER_QUERY_TYPE_REGISTER_REQ);
	ApplicationInfo info;
	info.type = ApplicationType::APPLICATION_TYPE_TSS;
	info.name = "Clock";
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

void Clock::sendQueryMessage()
{
	//构建查询消息
	RegisterQueryParser queryParser;
	queryParser.setCommandType(RegisterQueryType::REGISTER_QUERY_TYPE_QUERY_REQ);
	const std::string queryData{queryParser.compose()};

	//发送"query://sender=sourceID & data=X"
	UrlParser urlParser;
	urlParser.setCommand("query");
	urlParser.setCommandParameter("sender", sourceID);
	urlParser.setCommandParameter("data", queryData);
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

void Clock::sendClockMessage(const std::string clock)
{
	//构建通知消息
	NotifyParser notifyParser;
	notifyParser.setNotifyType(NotifyType::NOTIFY_TYPE_CLOCK_SYC);
	notifyParser.setClockInfo(clock);
	const std::string notifyData{notifyParser.compose()};

	//发送"notify://sender=sourceID & receiver=alarmApplicationID &data=X"
	UrlParser urlParser;
	urlParser.setCommand("notify");
	urlParser.setCommandParameter("sender", sourceID);
	urlParser.setCommandParameter("receiver", alarmApplicationID);
	urlParser.setCommandParameter("data", notifyData);
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

void Clock::processRegisterMessage(void* parser /*= nullptr*/)
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

void Clock::processQueryMessage(void* parser /*= nullptr*/)
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
		RegisterQueryParser queryParser;
		CommonError e{
			static_cast<CommonError>(queryParser.parse(data))};
		RegisterQueryType type{queryParser.getCommandType()};

		if (CommonError::COMMON_ERROR_SUCCESS == e)
		{
			if (RegisterQueryType::REGISTER_QUERY_TYPE_QUERY_REP == type)
			{
				std::vector<ApplicationInfo> infos{queryParser.getQueryApplicationInfos()};
				for (int i = 0; i != infos.size(); ++i)
				{
					if (ApplicationType::APPLICATION_TYPE_ALARM == infos[i].type)
					{
						alarmApplicationID = infos[i].parentID;
						logObj.write(
							framework::liblog::LogLevel::LOG_LEVEL_INFO, 
							"Got alarm application ID = [ %s ].",
							alarmApplicationID.c_str());
						break;
					}
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
