#include "boost/bind.hpp"
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
#include "HKDComponentClient.h"
using AsynchronousClientPtr = boost::shared_ptr<mq::module::AsynchronousClient>;

#include "Device/Hikvision/HikvisionDevice.h"
using AbstractDevice = base::device::AbstractDevice;
using HikvisionDevice = base::device::HikvisionDevice;

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

		//优先使用配置文件中的名称,如果配置文件中的名称空再使用参数列表中的名称
		//名称在第一次启动设置后就不能再修改
		std::string configName;
		XMLParser().getValueByName("Config.xml", "Component.HKD.Name", configName);
		const char* name{ cl.getParameterByOption("name") };
		if (configName.empty())
		{
			if (name)
			{
				configName.append(name);
				//名字直接写入配置文件,需要再去读
				XMLPacker().setValueWithName("Config.xml", "Component.HKD.Name", name);
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
	int e{ gHKDeviceComponentClientPtr ? eObjectExisted : eSuccess };

	if (eSuccess == e)
	{
		if (!gMessageDispatcherCenterIP.empty() && gMinPortNumber < gMessageDispatcherCenterPort && gMaxPortNumber > gMessageDispatcherCenterPort)
		{
			AsynchronousClientPtr acp{ boost::make_shared<HKDComponentClient>() };
			if (acp)
			{
				e = acp->startClient(gMessageDispatcherCenterIP.c_str(), gMessageDispatcherCenterPort);

				if (eSuccess == e)
				{
					gHKDeviceComponentClientPtr.swap(acp);
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

// static int testLoginDevice(void)
// {
// 	base::device::AbstractDevice* abp{ 
// 		new base::device::HikvisionDevice("admin", "eaton12345", "192.168.0.200", 8000) };
// 	if (abp)
// 	{
// 		if (eSuccess == abp->createNewDevice())
// 		{
// //			base::device::HikvisionDevice* hkdp{ std::dynamic_pointer_cast<base::device::HikvisionDevice*>(abp) };
// 			((base::device::HikvisionDevice*)abp)->getDeviceConfig();
// 		}
// 		else
// 		{
// 		}
// 	}
// }

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

	int e{ createNewAsynchronousClient() };
	if (eSuccess == e)
	{
		LOG(INFO) << "Create new asynchronous client result = " << e << ".";
//		testLoginDevice();
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