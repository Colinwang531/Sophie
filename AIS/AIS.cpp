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
#include "libais/ais.h"
using AISParser = framework::libais::AISParser;
#include "ais.h"

AIS::AIS(
	Log& log, 
	const std::string appID, 
	void* ctx /*= nullptr*/) 
	: Worker(ctx), logObj{log}, applicationID{appID}, 
	sourceID{Uuid().generateRandomUuid()}, tid{nullptr}, stopped{false}
{}
AIS::~AIS()
{}

int AIS::start(
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
			boost::bind(&AIS::timerTaskThreadHandler, this));
		e = static_cast<CommonError>(
			com.start(number, baudrate, boost::bind(&AIS::afterComReadDataHandler, this, _1, _2)));

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

int AIS::stop()
{
	stopped = true;
	com.stop();
	ThreadPool().destroy(tid);
	tid = nullptr;
	return static_cast<int>(CommonError::COMMON_ERROR_SUCCESS);
}

void AIS::afterWorkerPollDataProcess(const std::string data)
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

void AIS::afterComReadDataHandler(
	const char* data /* = nullptr */, 
	const int bytes /* = 0 */)
{
	if (data && 0 < bytes)
	{
		aisData.append(data, bytes);

		if ('\n' == *data)
		{
			if (!aisData.empty())
			{
				std::vector<std::string> items;
				boost::split(items, aisData, boost::is_any_of(","));
				const std::string flags{ items[0] };

				if ('$' == flags[0] || 'V' == flags[3] || 'D' == flags[4] || 'O' == flags[5])
				{
					const int totalNumber{ atoi(items[1].c_str()) };
					const int currentNumber{ atoi(items[2].c_str()) };

					if (totalNumber > currentNumber)
					{
						parseData.append(items[5]);
					}
					else if (totalNumber == currentNumber)
					{
						parseData.append(items[5]);
						logObj.write(
							framework::liblog::LogLevel::LOG_LEVEL_INFO, 
							"Got message [ %s ] from Com.", 
							parseData.c_str());
						sendNotifyMessage(parseData);
					}
				}
			}

			aisData.clear();
		}
	}
}

void AIS::timerTaskThreadHandler()
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

void AIS::sendRegisterMessage()
{
	//构建注册消息
	RegisterQueryParser registerParser;
	registerParser.setCommandType(RegisterQueryType::REGISTER_QUERY_TYPE_REGISTER_REQ);
	ApplicationInfo info;
	info.type = ApplicationType::APPLICATION_TYPE_AIS;
	info.name = "AIS";
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

void AIS::sendQueryMessage()
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

void AIS::sendNotifyMessage(const std::string data)
{
	//构建通知消息
	NotifyParser notifyParser;
	AISParser aisParser;
	CommonError e{
		static_cast<CommonError>(aisParser.parse(parseData))};

	if (CommonError::COMMON_ERROR_SUCCESS == e)
	{
		const AISDataType type{aisParser.getDataType()};
		notifyParser.setNotifyType(static_cast<NotifyType>(type));
		if(AISDataType::AIS_DATA_TYPE_POSITION_A == type)
		{
			notifyParser.setAISPositionAInfo(aisParser.getPositionAInfo());
		}
		else if(AISDataType::AIS_DATA_TYPE_POSITION_B == type)
		{
			notifyParser.setAISPositionBInfo(aisParser.getPositionBInfo());
		}
		else if(AISDataType::AIS_DATA_TYPE_SHIP_STATIC == type)
		{
			notifyParser.setAISShipStaticInfo(aisParser.getShipStaticInfo());
		}
		else if(AISDataType::AIS_DATA_TYPE_STANDARD_SAR == type)
		{
			notifyParser.setAISStandardSARInfo(aisParser.getStandardSARInfo());
		}
		else if(AISDataType::AIS_DATA_TYPE_AIDS_TO_NAVIGATION == type)
		{
			notifyParser.setAISAidsToNavigationInfo(aisParser.getAidsToNavigationInfo());
		}
		else
		{
			logObj.write(
				framework::liblog::LogLevel::LOG_LEVEL_WARNING, 
				"Not support AIS data type = [ %d ].", 
				static_cast<int>(type));
			return;
		}
	}
	else
	{
		logObj.write(
			framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
			"Parse message [ %s ] failed, result = [ %d ].", 
			parseData.c_str(),
			static_cast<int>(e));
	}
	
	const std::string notifyData{notifyParser.compose()};
	//发送"notify://sender=sourceID & receiver=webApplicationID &data=X"
	UrlParser urlParser;
	urlParser.setCommand("notify");
	urlParser.setCommandParameter("sender", sourceID);
	urlParser.setCommandParameter("receiver", webApplicationID);
	urlParser.setCommandParameter("data", notifyData);
	const std::string urlData{urlParser.compose()};
	int err{Worker::send(urlData)};

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
			err);
	}
}

void AIS::processRegisterMessage(void* parser /*= nullptr*/)
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

void AIS::processQueryMessage(void* parser /*= nullptr*/)
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
					if (ApplicationType::APPLICATION_TYPE_WEB == infos[i].type)
					{
						webApplicationID = infos[i].parentID;
						logObj.write(
							framework::liblog::LogLevel::LOG_LEVEL_INFO, 
							"Got alarm application ID = [ %s ].",
							webApplicationID.c_str());
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
