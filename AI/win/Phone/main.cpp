#include <windows.h>
#include "boost/bind.hpp"
#include "boost/format.hpp"
#include "boost/make_shared.hpp"
#ifdef _WINDOWS
#include "glog/log_severity.h"
#include "glog/logging.h"
#else
#include "glog/log_severity.h"
#include "glog/logging.h"
#endif//_WINDOWS
#include "Error.h"
#include "Define.h"
#include "CommandLine/CommandLine.h"
using CommandLine = base::commandline::CommandLine;
#include "Xml/XmlCodec.h"
using XMLParser = base::xml::XMLParser;
using XMLPacker = base::xml::XMLPacker;
#include "PhoneDetectComponentClient.h"
using PhoneDetectComponentClientPtr = boost::shared_ptr<base::network::AbstractClient>;
#include "Device/Hikvision/HikvisionDevice.h"
using AbstractDevice = base::device::AbstractDevice;
using HikvisionDevice = base::device::HikvisionDevice;

static std::string gMessageDispatcherCenterIP{ "127.0.0.1" };
static unsigned short gMessageDispatcherCenterPort{ 61001 };
PhoneDetectComponentClientPtr gPhoneDetectComponentClientPtr;

static void parseCommandLine(int argc, char** argv)
{
	CommandLine cl;
	cl.setCommandOptions("address,a", "127.0.0.1");
	cl.setCommandOptions("port,p", "61001");
	cl.setCommandOptions("name,n", "Phone");

	if (eSuccess == cl.parseCommandLine(argc, argv))
	{
		const char* address{ cl.getParameterByOption("address") };
		if (address)
		{
			gMessageDispatcherCenterIP.clear();
			gMessageDispatcherCenterIP.append(address);
		}

		const char* port{ cl.getParameterByOption("port") };
		if (port)
		{
			gMessageDispatcherCenterPort = static_cast<unsigned short>(atoi(port));
		}

		//优先使用配置文件中的名称,如果配置文件中的名称空再使用参数列表中的名称
		//名称在第一次启动设置后就不能再修改
		std::string configName;
		XMLParser().getValueByName("Config.xml", "Component.Phone.Name", configName);
		const char* name{ cl.getParameterByOption("name") };
		if (configName.empty())
		{
			if (name)
			{
				configName.append(name);
				//名字直接写入配置文件,需要再去读
				XMLPacker().setValueWithName("Config.xml", "Component.Phone.Name", configName);
			}
		}

		LOG(INFO) <<
			"Remote server address is " << gMessageDispatcherCenterIP <<
			", server port is " << gMessageDispatcherCenterPort <<
			", name is " << configName << ".";
	}
}

static int createNewAsynchronousClient(void)
{
	int e{ gPhoneDetectComponentClientPtr ? eObjectExisted : eSuccess };

	if (eSuccess == e)
	{
		if (!gMessageDispatcherCenterIP.empty() && gMinPortNumber < gMessageDispatcherCenterPort && gMaxPortNumber > gMessageDispatcherCenterPort)
		{
			PhoneDetectComponentClientPtr acp{ 
				boost::make_shared<PhoneDetectComponentClient>(gMessageDispatcherCenterIP) };
			if (acp)
			{
				e = acp->startClient(
					(boost::format("tcp://%s:%d") % gMessageDispatcherCenterIP % gMessageDispatcherCenterPort).str().c_str());

				if (eSuccess == e)
				{
					gPhoneDetectComponentClientPtr.swap(acp);
				}
				else
				{
					e = eBadOperate;
				}
			}
			else
			{
				e = eBadNewObject;
			}
		}
		else
		{
			e = eInvalidParameter;
		}
	}

	return e;
}

static int destroyAsynchronousClient(void)
{
	int e{ gPhoneDetectComponentClientPtr ? eSuccess : eBadOperate };

	if (eSuccess == e)
	{
		e = gPhoneDetectComponentClientPtr->stopClient();

		if (eSuccess == e)
		{
			gPhoneDetectComponentClientPtr.reset();
		}
		else
		{
			e = eBadOperate;
		}
	}

	return e;
}

int main(int argc, char** argv)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	WSAStartup(wVersionRequested, &wsaData);

	FLAGS_stderrthreshold = GLOG_INFO;
	FLAGS_colorlogtostderr = 1;
	google::InitGoogleLogging(argv[0]);
	google::SetLogDestination(GLOG_INFO,
#ifdef _WINDOWS
		".\\"
#else
		"./"
#endif
	);

	parseCommandLine(argc, argv);

	int e{ createNewAsynchronousClient() };
	if (eSuccess == e)
	{
		LOG(INFO) << "Create new asynchronous client result = " << e << ".";
	}
	else
	{
		LOG(WARNING) << "Create new asynchronous client result = " << e << ".";
	}

	getchar();

	e = destroyAsynchronousClient();
	if (eSuccess == e)
	{
		LOG(INFO) << "Destroy asynchronous client result = " << e << ".";
	}
	else
	{
		LOG(WARNING) << "Destroy asynchronous client result = " << e << ".";
	}

	google::ShutdownGoogleLogging();
	return 0;
}