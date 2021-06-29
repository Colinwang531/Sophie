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
#include "libcommon/const.h"
#include "libcommon/error.h"
#include "CMS.h"

CMS::CMS(
	Log& log, 
	const std::string id, 
	void* ctx /*= nullptr*/) 
	: Worker(ctx), logObj{log}, applicationID{id}, 
	sourceID{Uuid().generateRandomUuid()}, tid{nullptr}, stopped{false}
{}
CMS::~CMS()
{
	stopped = true;
	ThreadPool().destroy(tid);
	tid = nullptr;	
}

int CMS::connect(
	const std::string remoteIP,
	const unsigned short remotePort)
{
	CommonError e{
		static_cast<CommonError>(Worker::connect(sourceID, remoteIP, remotePort))};

	if (CommonError::COMMON_ERROR_SUCCESS == e)
	{
		logObj.write(
			framework::liblog::LogLevel::LOG_LEVEL_INFO, 
			"Connect remote IP [ %s ] and Port [ %d ] with ID [ %s ] successfully.", 
			remoteIP.c_str(), 
			remotePort,
			sourceID.c_str());

		tid = ThreadPool().create(
			boost::bind(&CMS::timerTaskHandler, this));
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

int CMS::setup(
	const std::string uploadIP,
	const unsigned short uploadPort)
{
	//发送"setup://address=uploadIP&port=uploadPort"
	UrlParser parser;
	parser.setCommand("setup");
	parser.setCommandParameter("address", uploadIP);
	parser.setCommandParameter("port", (boost::format("%d") % uploadPort).str());
	const std::string data{parser.compose()};
	int e{Worker::send(data)};

	if (CommonError::COMMON_ERROR_SUCCESS == static_cast<CommonError>(e))
	{
		logObj.write(
			framework::liblog::LogLevel::LOG_LEVEL_INFO, 
			"Send setup message [ %s ] successfully.", 
			data.c_str());
	}
	else
	{
		logObj.write(
			framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
			"Send setup message [ %s ] failed, result = [ %d ].", 
			data.c_str(),
			e);
	}

	return static_cast<int>(e);
}

void CMS::afterWorkerPollDataProcess(
	const std::string data)
{
	if (!data.empty())
	{
		UrlParser parser;
		int e{parser.parse(data)};

		if (CommonError::COMMON_ERROR_SUCCESS == static_cast<CommonError>(e))
		{
			const std::string command{parser.getCommand()};

			if (0 == command.compare("pair"))
			{
				processPairMessage(&parser);
			}
			else if (0 == command.compare("register"))
			{
				processRegisterMessage(&parser);
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

void CMS::timerTaskHandler()
{
	unsigned long long lastTick{0};

	while (!stopped)
	{
		unsigned long long nowTick{Time().tickcount()};

		if (30000 < nowTick - lastTick)
		{
			sendPairMessage();
			lastTick = nowTick;
		}
	}
}

void CMS::processPairMessage(void* parser /*= nullptr*/)
{
	UrlParser* urlparser{reinterpret_cast<UrlParser*>(parser)};
	const std::vector<ParamItem> commandParameters{urlparser->getCommandParameters()};

	//遍历命令参数段
	//找出sender参数
	//重构url

	for (int i = 0; i != commandParameters.size(); ++i)
	{
		if (0 == commandParameters[i].key.compare("sender"))
		{
			sendRegisterMessage(commandParameters[i].value);
			break;
		}
	}
}

void CMS::processRegisterMessage(void* parser /*= nullptr*/)
{
	UrlParser* urlparser{reinterpret_cast<UrlParser*>(parser)};
	const std::vector<ParamItem> commandParamItems{urlparser->getCommandParameters()};
	std::string sender, via;

	for (int i = 0; i != commandParamItems.size(); ++i)
	{
		if (0 == commandParamItems[i].key.compare("sender"))
		{
			sender = commandParamItems[i].value;
		}
		else if (0 == commandParamItems[i].key.compare("via"))
		{
			via = commandParamItems[i].value;
		}
	}

	//注册应答"register:\\sender=sourceID&receiver=sender"
	//或"register:\\sender=sourceID&receiver=sender?via=X"
	UrlParser newUrlParser;
	newUrlParser.setCommand("register");
	newUrlParser.setCommandParameter("sender", sourceID);
	newUrlParser.setCommandParameter("receiver", sender);
	if (!via.empty())
	{
		newUrlParser.setUserParameter("via", via);
	}
	const std::string data{newUrlParser.compose()};
	int e{Worker::send(data)};

	if (CommonError::COMMON_ERROR_SUCCESS == static_cast<CommonError>(e))
	{
		logObj.write(
			framework::liblog::LogLevel::LOG_LEVEL_INFO, 
			"Send register message [ %s ] successfully.", 
			data.c_str());
	}
	else
	{
		logObj.write(
			framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
			"Send register message [ %s ] failed, result = [ %d ].", 
			data.c_str(),
			e);
	}
}

void CMS::sendPairMessage()
{
	//发送"pair://"
	UrlParser parser;
	parser.setCommand("pair");
	const std::string data{parser.compose()};
	int e{Worker::send(data)};

	if (CommonError::COMMON_ERROR_SUCCESS == static_cast<CommonError>(e))
	{
		logObj.write(
			framework::liblog::LogLevel::LOG_LEVEL_INFO, 
			"Send message [ %s ] successfully.", 
			data.c_str());
	}
	else
	{
		logObj.write(
			framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
			"Send message [ %s ] failed, result = [ %d ].", 
			data.c_str(),
			e);
	}
}

void CMS::sendRegisterMessage(const std::string viaID)
{
	//发送"register://sender=sourceID&via=viaID?upload=true"
	UrlParser parser;
	parser.setCommand("pair");
	const std::string data{parser.compose()};
	int e{Worker::send(data)};

	if (CommonError::COMMON_ERROR_SUCCESS == static_cast<CommonError>(e))
	{
		logObj.write(
			framework::liblog::LogLevel::LOG_LEVEL_INFO, 
			"Send message [ %s ] successfully.", 
			data.c_str());
	}
	else
	{
		logObj.write(
			framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
			"Send message [ %s ] failed, result = [ %d ].", 
			data.c_str(),
			e);
	}
}
