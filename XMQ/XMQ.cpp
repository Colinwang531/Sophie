#include "libcommon/uuid/uuid.h"
using Uuid = framework::libcommon::Uuid;
#include "liburl/url.h"
using UrlParser = framework::liburl::UrlParser;
using ParamItem = framework::liburl::ParamItem;
#include "libcommon/defs.h"
#include "libcommon/error.h"
#include "xmq.h"

XMQ::XMQ(
	Log& log, 
	const std::string appID, 
	void* ctx /*= nullptr*/) 
	: Switcher(ctx), Worker(ctx), logObj{log}, 
	sourceID{Uuid().generateRandomUuid()}, applicationID{appID}, isUpload{false}
{}
XMQ::~XMQ()
{}

int XMQ::bind(
	const std::string localIP, 
	const unsigned short localPort /*= 0*/)
{
	CommonError e{
		static_cast<CommonError>(
			Switcher::bind(localIP, localPort))};

	if (CommonError::COMMON_ERROR_SUCCESS == e)
	{
		logObj.write(
			framework::liblog::LogLevel::LOG_LEVEL_INFO, 
			"Bind local IP [ %s ] and Port [ %d ] for listening successfully.", 
			localIP.c_str(), 
			localPort);
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

int XMQ::connect(
	const std::string remoteIP,
	const unsigned short remotePort /*= 0*/)
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

void XMQ::afterSwitcherPollDataProcess(
	const std::string sourceID,
	const std::string data)
{
	if (!sourceID.empty() && !data.empty())
	{
		UrlParser parser;
		int e{parser.parse(data)};

		if (CommonError::COMMON_ERROR_SUCCESS == static_cast<CommonError>(e))
		{
			const std::string command{parser.getCommand()};

			if (0 == command.compare("register") || 
				0 == command.compare("query") || 
				0 == command.compare("config") || 
				0 == command.compare("notify") || 
				0 == command.compare("alarm"))
			{
				//参数处理流程via -> upload -> receiver -> friend
				if (!forwardVia(&parser))
				{
					if (!forwardUpload(sourceID, &parser))
					{
						if (!forwardReceiver(&parser))
						{
							forwardFriend(data);
						}
					}
				}
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

void XMQ::afterWorkerPollDataProcess(
	const std::string data)
{
	if (!data.empty())
	{
		UrlParser parser;
		int e{parser.parse(data)};

		if (CommonError::COMMON_ERROR_SUCCESS == static_cast<CommonError>(e))
		{
			const std::string command{parser.getCommand()};

			if (0 == command.compare("register") || 
				0 == command.compare("query") || 
				0 == command.compare("config") || 
				0 == command.compare("notify") || 
				0 == command.compare("alarm"))
			{
				//参数处理流程via -> receiver -> upload -> friend
				if (!forwardVia(&parser))
				{
					if (!forwardReceiver(&parser))
					{
						if (!forwardUpload(sourceID, &parser))
						{
							forwardFriend(data);
						}
					}
				}
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

bool XMQ::forwardVia(void* parser /*= nullptr*/)
{
	bool result{false};
	UrlParser* urlparser{reinterpret_cast<UrlParser*>(parser)};
	std::vector<ParamItem> userParameters{urlparser->getUserParameters()};

	//遍历用户参数段
	//找出第一个出现的via参数
	//删除第一个出现的via参数
	//在用户参数段尾部添加upload=true参数
	//重构url并转发到via参数值的目标

	for (std::vector<ParamItem>::iterator it = userParameters.begin();
		 it != userParameters.end();)
	{
		if (0 == it->key.compare("via"))
		{
			result = true;
			const std::string receiver{it->value};
			userParameters.erase(it);

			UrlParser newUrlParser;
			newUrlParser.setCommand(urlparser->getCommand());
			const std::vector<ParamItem> commandParameters{urlparser->getCommandParameters()};
			for (int j = 0; j != commandParameters.size(); ++j)
			{
				newUrlParser.setCommandParameter(commandParameters[j].key, commandParameters[j].value);
			}
			for (int k = 0; k != userParameters.size(); ++k)
			{
				newUrlParser.setUserParameter(userParameters[k].key, userParameters[k].value);
			}
			newUrlParser.setUserParameter("upload", "true");

			const std::string data{newUrlParser.compose()};
			int e{Switcher::send(receiver, data)};

			if (CommonError::COMMON_ERROR_SUCCESS == static_cast<CommonError>(e))
			{
				logObj.write(
					framework::liblog::LogLevel::LOG_LEVEL_INFO, 
					"Forward message [ %s ] to target [ %s ] successfully.", 
					data.c_str(),
					receiver.c_str());
			}
			else
			{
				logObj.write(
					framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
					"Forward message [ %s ] to target [ %s ] failed, result = [ %d ].", 
					data.c_str(),
					receiver.c_str(),
					e);
			}

			break;
		}

		++it;
	}
	

	return result;
}

bool XMQ::forwardUpload(
	const std::string sourceID, 
	void* parser /*= nullptr*/)
{
	bool status{false};
	UrlParser* urlparser{reinterpret_cast<UrlParser*>(parser)};

	//遍历查找address、port、data和upload字段
	//连接upload
	//发送"register://sender=sourceID & data=X & via=this->sourceID"

	std::string address, port, data;
	std::vector<ParamItem> commandParameters{urlparser->getCommandParameters()};
	for (int i = 0; i != commandParameters.size(); ++i)
	{
		if (0 == commandParameters[i].key.compare("address"))
		{
			address = commandParameters[i].value;
		}
		else if (0 == commandParameters[i].key.compare("port"))
		{
			port = commandParameters[i].value;
		}
		else if (0 == commandParameters[i].key.compare("data"))
		{
			data = commandParameters[i].value;
		}
	}

	std::vector<ParamItem> userParameters{urlparser->getUserParameters()};
	for (int i = 0; i != userParameters.size(); ++i)
	{
		if (0 == userParameters[i].key.compare("upload"))
		{
			status = true;
			friendID = sourceID;
			break;
		}
	}

	if (status)
	{
		if (!isUpload && !address.empty() && !port.empty())
		{
			isUpload = (
				CommonError::COMMON_ERROR_SUCCESS == static_cast<CommonError>(XMQ::connect(address, atoi(port.c_str()))) ? 
				true : 
				false);
		}

		if (isUpload)
		{
			UrlParser newUrlParser;
			newUrlParser.setCommand(urlparser->getCommand());
			newUrlParser.setCommandParameter("sender", sourceID);
			newUrlParser.setCommandParameter("data", data);
			newUrlParser.setCommandParameter("via", this->sourceID);
			const std::string urlData{newUrlParser.compose()};
			int e{Worker::send(urlData)};

			if (CommonError::COMMON_ERROR_SUCCESS == static_cast<CommonError>(e))
			{
				logObj.write(
					framework::liblog::LogLevel::LOG_LEVEL_INFO, 
					"Forward message [ %s ] successfully.", 
					data.c_str());
			}
			else
			{
				logObj.write(
					framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
					"Forward message [ %s ] failed, result = [ %d ].", 
					data.c_str(), 
					e);
			}
		}
	}
	
	return status;
}

bool XMQ::forwardReceiver(void* parser /*= nullptr*/)
{
	bool result{false};
	UrlParser* urlparser{reinterpret_cast<UrlParser*>(parser)};
	const std::vector<ParamItem> commandParameters{urlparser->getCommandParameters()};

	//遍历命令参数段
	//找出receiver参数
	//重构url并转发到receiver参数值的目标

	for (int i = 0; i != commandParameters.size(); ++i)
	{
		if (0 == commandParameters[i].key.compare("receiver"))
		{
			result = true;
			const std::string data{urlparser->compose()};
			int e{Switcher::send(commandParameters[i].value, data)};

			if (CommonError::COMMON_ERROR_SUCCESS == static_cast<CommonError>(e))
			{
				logObj.write(
					framework::liblog::LogLevel::LOG_LEVEL_INFO, 
					"Forward message [ %s ] to target [ %s ] successfully.", 
					data.c_str(),
					commandParameters[i].value.c_str());
			}
			else
			{
				logObj.write(
					framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
					"Forward message [ %s ] to target [ %s ] failed, result = [ %d ].", 
					data.c_str(),
					commandParameters[i].value.c_str(),
					e);
			}

			break;
		}
	}

	return result;
}

void XMQ::forwardFriend(const std::string data)
{
	if (!friendID.empty())
	{
		CommonError e{
			static_cast<CommonError>(Switcher::send(friendID, data))};

		if (CommonError::COMMON_ERROR_SUCCESS == e)
		{
			logObj.write(
				framework::liblog::LogLevel::LOG_LEVEL_INFO, 
				"Forward message to friend [ %s ] successfully.", 
				friendID.c_str());
		}
		else
		{
			logObj.write(
				framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
				"Forward message to friend [ %s ] failed, result = [ %d ].", 
				friendID.c_str(),
				e);
		}
	}
	else
	{
		logObj.write(
			framework::liblog::LogLevel::LOG_LEVEL_WARNING, 
			"Can not forward message to friend [ %s ].", 
			friendID.c_str());
	}
}
