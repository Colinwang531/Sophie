#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#ifdef _WINDOWS
#include "glog/log_severity.h"
#include "glog/logging.h"
#else
#include "glog/log_severity.h"
#include "glog/logging.h"
#endif//_WINDOWS
#include "Define.h"
#include "Error.h"
#include "CommandLine/CommandLine.h"
using CommandLine = base::commandline::CommandLine;
#include "HKDCClient.h"
using AsynchronousClientPtr = boost::shared_ptr<mq::module::AsynchronousClient>;
static std::string gHKDeviceCompnentName{ "Hikvision Device Component" };
static std::string gMessageDispatcherCenterIP{ "127.0.0.1" };
static unsigned short gMessageDispatcherCenterPort{ 61001 };
static AsynchronousClientPtr gHKDeviceComponentClientPtr{ nullptr };

static void parseCommandLine(int argc, char** argv)
{
	CommandLine cl;
	cl.setCommandOptions("address,a", "127.0.0.1");
	cl.setCommandOptions("port,p", "61001");
	cl.setCommandOptions("name,n", "Unknown");

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

		const char* name{ cl.getParameterByOption("name") };
		if (name)
		{
			gHKDeviceCompnentName.clear();
			gHKDeviceCompnentName.append(name);
		}
	}
}

static int createNewAsynchronousClient(void)
{
	int e{ gHKDeviceComponentClientPtr ? eObjectExisted : eSuccess };

	if (eSuccess == e)
	{
		if (!gMessageDispatcherCenterIP.empty() && gMinPortNumber < gMessageDispatcherCenterPort && gMaxPortNumber > gMessageDispatcherCenterPort)
		{
			AsynchronousClientPtr ac{ boost::make_shared<HKDCClient>(gHKDeviceCompnentName) };
			if (ac)
			{
				e = ac->startClient(gMessageDispatcherCenterIP.c_str(), gMessageDispatcherCenterPort);

				if (eSuccess == e)
				{
					gHKDeviceComponentClientPtr.swap(ac);
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
	int e{ gHKDeviceComponentClientPtr ? eSuccess : eBadOperate };

	if (eSuccess == e)
	{
		e = gHKDeviceComponentClientPtr->stopClient();

		if (eSuccess == e)
		{
			gHKDeviceComponentClientPtr.reset();
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
	FLAGS_stderrthreshold = INFO;
	FLAGS_colorlogtostderr = 1;
	google::InitGoogleLogging(argv[0]);
	google::SetLogDestination(INFO,
#ifdef _WINDOWS
		".\\"
#else
		"./"
#endif
	);

	parseCommandLine(argc, argv);
	LOG(INFO) << "Remote server address is " << gMessageDispatcherCenterIP << ", server port is " << gMessageDispatcherCenterPort;

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