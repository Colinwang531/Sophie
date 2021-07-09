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
#include "cms.h"

CMS::CMS(
	Log& log, 
	const std::string appID, 
	void* ctx /*= nullptr*/) 
	: Worker(ctx), logObj{log}, applicationID{appID}, 
	sourceID{Uuid().generateRandomUuid()}, tid{nullptr}, stopped{false}, uploadPort{0}
{}
CMS::~CMS()
{}

int CMS::start(
	const std::string remoteIP,
	const unsigned short remotePort,
	const std::string uploadIP,
	const unsigned short uploadPort)
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

		this->uploadIP = uploadIP;
		this->uploadPort = uploadPort;
		tid = ThreadPool().create(
			boost::bind(&CMS::timerTaskThreadHandler, this));
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

int CMS::stop()
{
	stopped = true;
	ThreadPool().destroy(tid);
	tid = nullptr;
	return static_cast<int>(CommonError::COMMON_ERROR_SUCCESS);
}

void CMS::afterWorkerPollDataProcess(const std::string data)
{
	if (!data.empty())
	{
		UrlParser parser;
		int e{parser.parse(data)};

		if (CommonError::COMMON_ERROR_SUCCESS == static_cast<CommonError>(e))
		{
			const std::string command{parser.getCommand()};

			//CMS组件只处理register和query命令
			//其他命令由组件自行交互处理
			if (0 == command.compare("register") || 0 == command.compare("query"))
			{
				processRegisterQueryMessage(&parser);
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

void CMS::timerTaskThreadHandler()
{
	unsigned long long lastTick{0};

	while (!stopped)
	{
		unsigned long long nowTick{Time().tickcount()};

		if (30000 < nowTick - lastTick)
		{
			sendRegisterMessage();
			removeExpiredApplicationInfo();
			lastTick = nowTick;
		}
	}
}

void CMS::processRegisterQueryMessage(void* parser /*= nullptr*/)
{
	UrlParser* urlparser{reinterpret_cast<UrlParser*>(parser)};
	const std::vector<ParamItem> commandParamItems{urlparser->getCommandParameters()};
	//如果命令参数中包含via字段则表示该消息是跨域交互
	//否则表示本域交互，且必须包含receiver字段
	//无论是跨域还是本域交互，sender和data字段是必须包含的
	std::string sender, data, target;
	bool hasVia{false};

	for (int i = 0; i != commandParamItems.size(); ++i)
	{
		if (0 == commandParamItems[i].key.compare("sender"))
		{
			sender = commandParamItems[i].value;
		}
		else if (0 == commandParamItems[i].key.compare("data"))
		{
			data = commandParamItems[i].value;
		}
		else if (0 == commandParamItems[i].key.compare("via"))
		{
			hasVia = true;
			target = commandParamItems[i].value;
		}
		else if (0 == commandParamItems[i].key.compare("receiver"))
		{
			target = commandParamItems[i].value;
		}
	}

	processRegisterQueryMessage(sender, data, target, hasVia);
}

void CMS::processRegisterQueryMessage(
	const std::string sender, 
	const std::string data, 
	const std::string target, 
	const bool isVia /*= false*/)
{
	RegisterQueryParser parser;
	CommonError e{
		static_cast<CommonError>(parser.parse(data))};
	RegisterQueryType type{parser.getCommandType()};

	if (CommonError::COMMON_ERROR_SUCCESS == e)
	{
		if (RegisterQueryType::REGISTER_QUERY_TYPE_REGISTER_REQ == type)
		{
			updateRegisterApplicationInfo(sender, target, isVia, &parser);
			sendRegisterReplyMessage(sender, target, isVia);
		}
		else if (RegisterQueryType::REGISTER_QUERY_TYPE_REGISTER_REP == type)
		{
			const int statusCode{parser.getStatusCode()};
			if (CommonError::COMMON_ERROR_SUCCESS == static_cast<CommonError>(statusCode))
			{
				logObj.write(
					framework::liblog::LogLevel::LOG_LEVEL_INFO, 
					"Receive message of registration successfully.");
			}
			else
			{
				logObj.write(
					framework::liblog::LogLevel::LOG_LEVEL_WARNING, 
					"Receive message of registration failed, result = [ %d ].",
					statusCode);
			}
		}
		else if (RegisterQueryType::REGISTER_QUERY_TYPE_QUERY_REQ == type)
		{
			sendQueryReplyMessage(sender, target, isVia);
		}
		else
		{
			logObj.write(
				framework::liblog::LogLevel::LOG_LEVEL_WARNING, 
				"Add unknown registration type = [ %d ] failed.", 
				static_cast<int>(type));
		}
	}
	else
	{
		logObj.write(
			framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
			"Parse register message [ %s ] failed, result = [ %d ].", 
			data.c_str(),
			static_cast<int>(e));
	}
}

void CMS::sendRegisterMessage()
{
	//构建注册消息
	RegisterQueryParser registerParser;
	registerParser.setCommandType(RegisterQueryType::REGISTER_QUERY_TYPE_REGISTER_REQ);
	ApplicationInfo info;
	info.type = ApplicationType::APPLICATION_TYPE_CMS;
	info.name = "CMS";
	info.ID = applicationID;
	registerParser.setRegisterApplicationInfo(info);
	const std::string registerData{registerParser.compose()};

	//发送"register://address=uploadIP & port=uploadPort & data=X ? upload=true"
	UrlParser parser;
	parser.setCommand("register");
	parser.setCommandParameter("address", uploadIP);
	parser.setCommandParameter("port", (boost::format("%d") % uploadPort).str());
	parser.setCommandParameter("data", registerData);
	parser.setUserParameter("upload", "true");
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

void CMS::sendRegisterReplyMessage(
	const std::string receiver, 
	const std::string via, 
	const bool isVia /*= false*/)
{
	RegisterQueryParser registerParser;
	registerParser.setCommandType(RegisterQueryType::REGISTER_QUERY_TYPE_REGISTER_REP);
	registerParser.setStatusCode(static_cast<int>(CommonError::COMMON_ERROR_SUCCESS));
	const std::string registerData{registerParser.compose()};

	//注册应答"register://sender=sourceID & receiver=sender & data=X"
	//或"register://sender=sourceID & receiver=sender & data=X ? via=Y"
	UrlParser newUrlParser;
	newUrlParser.setCommand("register");
	newUrlParser.setCommandParameter("sender", sourceID);
	newUrlParser.setCommandParameter("receiver", receiver);
	newUrlParser.setCommandParameter("data", registerData);
	if (!via.empty())
	{
		newUrlParser.setUserParameter("via", via);
	}
	const std::string urlData{newUrlParser.compose()};
	int e{Worker::send(urlData)};

	if (CommonError::COMMON_ERROR_SUCCESS == static_cast<CommonError>(e))
	{
		logObj.write(
			framework::liblog::LogLevel::LOG_LEVEL_INFO, 
			"Send register reply message [ %s ] successfully.", 
			urlData.c_str());
	}
	else
	{
		logObj.write(
			framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
			"Send register reply message [ %s ] failed, result = [ %d ].", 
			urlData.c_str(),
			e);
	}
}

void CMS::sendQueryReplyMessage(
	const std::string receiver, 
	const std::string via, 
	const bool isVia /*= false*/)
{
	RegisterQueryParser registerParser;
	registerParser.setCommandType(RegisterQueryType::REGISTER_QUERY_TYPE_QUERY_REP);
	registerParser.setStatusCode(static_cast<int>(CommonError::COMMON_ERROR_SUCCESS));
	std::vector<ApplicationInfo> infos;
	for (int i = 0; i != applicationInfos.size(); ++i)
	{
		infos.push_back(applicationInfos.item(i));
	}
	registerParser.setQueryApplicationInfos(infos);
	
	const std::string registerData{registerParser.compose()};

	//查询应答"query://sender=sourceID & receiver=sender & data=X"
	//或"query://sender=sourceID & receiver=sender & data=X ? via=Y"
	UrlParser newUrlParser;
	newUrlParser.setCommand("query");
	newUrlParser.setCommandParameter("sender", sourceID);
	newUrlParser.setCommandParameter("receiver", receiver);
	newUrlParser.setCommandParameter("data", registerData);
	if (!via.empty())
	{
		newUrlParser.setUserParameter("via", via);
	}
	const std::string urlData{newUrlParser.compose()};
	int e{Worker::send(urlData)};

	if (CommonError::COMMON_ERROR_SUCCESS == static_cast<CommonError>(e))
	{
		logObj.write(
			framework::liblog::LogLevel::LOG_LEVEL_INFO, 
			"Send query reply message [ %s ] successfully.", 
			urlData.c_str());
	}
	else
	{
		logObj.write(
			framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
			"Send query reply message [ %s ] failed, result = [ %d ].", 
			urlData.c_str(),
			e);
	}
}

void CMS::removeExpiredApplicationInfo()
{
	const unsigned long long applicationInfosNumber{applicationInfos.size()};
	const unsigned long long now{Time().tickcount()};

	for(int i = 0; i != applicationInfosNumber; ++i)
	{
		ApplicationInfo info{applicationInfos.item(i)};
		const unsigned long long registerTimestamp{info.timestamp};

		if(30000 < now - registerTimestamp)
		{
			applicationInfos.removeItem(i);
			logObj.write(
				framework::liblog::LogLevel::LOG_LEVEL_WARNING, 
				"Remove expired application ID [ %s ], name = [ %s ], type = [ %d ].", 
				info.ID.c_str(),
				info.name.c_str(),
				static_cast<int>(info.type));
				
			break;
		}
	}
}

int CMS::updateRegisterApplicationInfo(
	const std::string sender,
	const std::string via, 
	const bool isVia /*= false*/,
	void* parser /*= nullptr*/)
{
	CommonError e{CommonError::COMMON_ERROR_SUCCESS};
	RegisterQueryParser* registerQueryParser{
		reinterpret_cast<RegisterQueryParser*>(parser)};
	ApplicationInfo info{
		registerQueryParser->getRegisterApplicationInfo()};

	if (ApplicationType::APPLICATION_TYPE_NONE < info.type)
	{
		//已存在的更新时间戳
		//不存在就添加
		info.timestamp = Time().tickcount();
		bool exist{false};

		for (int i = 0; i != applicationInfos.size(); ++i)
		{
			ApplicationInfo* infoRef{applicationInfos.itemRef(i)};
			if (0 == infoRef->ID.compare(info.ID))
			{
				infoRef->timestamp = Time().tickcount();
				exist = true;
			}
		}

		if(!exist)
		{
			if (isVia)
			{
				info.parentID = via;
			}
			
			applicationInfos.addItem(info);
			logObj.write(
				framework::liblog::LogLevel::LOG_LEVEL_INFO, 
				"Add new registration type = [ %d ], name = [ %s ], ID = [ %s], IPv4 = [ %s ] successfully.", 
				static_cast<int>(info.type),
				info.name.c_str(),
				info.ID.c_str(),
				info.IPv4.c_str());
		}
	}
	else
	{
		e = CommonError::COMMON_ERROR_SET_FAILED;
		logObj.write(
			framework::liblog::LogLevel::LOG_LEVEL_WARNING, 
			"Add unknown registration type = [ %d ], name = [ %s ], ID = [ %s], IPv4 = [ %s ] failed.", 
			static_cast<int>(info.type),
			info.name.c_str(),
			info.ID.c_str(),
			info.IPv4.c_str());
	}

	return static_cast<int>(e);
}
