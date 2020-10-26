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
#include "Xml/XmlCodec.h"
using XMLParser = base::xml::XMLParser;
using XMLPacker = base::xml::XMLPacker;
#include "CommandLine/CommandLine.h"
using CommandLine = base::commandline::CommandLine;
#include "MDCServer.h"
using MajordomoUpstreamBroker = base::network::MajordomoUpstreamBroker;
using MDCServerPtr = boost::shared_ptr<MajordomoUpstreamBroker>;

static std::string gServiceName;
static std::string gUpstreamIP;
static unsigned short gUpstreamPort{ 0 };
static unsigned short gListenPort{ 0 };
static MDCServerPtr gMDCServerPtr;

static void parseCommandLine(int argc, char** argv)
{
	CommandLine cl;
	cl.setCommandOptions("listen,l", "61001");
	cl.setCommandOptions("upstream,u", "127.0.0.1");
	cl.setCommandOptions("port,p", "61101");
	cl.setCommandOptions("name,n", "Dispatcher");

	if (eSuccess == cl.parseCommandLine(argc, argv))
	{
		const char* upstreamIP{ cl.getParameterByOption("upstream") };
		if (upstreamIP)
		{
			gUpstreamIP.append(upstreamIP);
		}

		const char* upstreamPort{ cl.getParameterByOption("port") };
		if (upstreamPort)
		{
			gUpstreamPort = atoi(upstreamPort);
		}
		
		const char* listenPort{ cl.getParameterByOption("listen") };
		if (listenPort)
		{
			gListenPort = atoi(listenPort);
		}

		//优先使用配置文件中的名称,如果配置文件中的名称空再使用参数列表中的名称
		//名称在第一次启动设置后就不能再修改
		std::string configName;
		XMLParser().getValueByName("Config.xml", "Component.Dispatcher.Name", configName);
		const char* name{ cl.getParameterByOption("name") };
		if (configName.empty())
		{
			if (name)
			{
				configName.append(name);
				//名字直接写入配置文件,需要再去读
				XMLPacker().setValueWithName("Config.xml", "Component.Dispatcher.Name", configName);
			}
		}

		LOG(INFO) << "Listen port = " << gListenPort <<
			", Name = " << configName <<
			", upstream address = " << gUpstreamIP <<
			", upstream port = " << gUpstreamPort << ".";
	}
}

static int createNewMDCServer(void)
{
	int e{ gMDCServerPtr ? eObjectExisted : eSuccess };

	MDCServerPtr mdcsp{ boost::make_shared<MDCServer>() };
	if (mdcsp)
	{
		e = mdcsp->startServer(gListenPort);
		if (eSuccess == e)
		{
			mdcsp->startClient((boost::format("tcp://%s:%d") % gUpstreamIP % gUpstreamPort).str().c_str());
			gMDCServerPtr.swap(mdcsp);
		}
	}
	else
	{
		e = eBadNewObject;
	}

	return e;
}

static int destroyMDCServer(void)
{
	int e{ gMDCServerPtr ? eSuccess : eBadOperate };

	if (eSuccess == e)
	{
		e = gMDCServerPtr->stopServer();

		if (eSuccess == e)
		{
			gMDCServerPtr->stopClient();
			gMDCServerPtr.reset();
		}
		else
		{
			e = eBadOperate;
		}
	}

	return e;
}

int main(int argc, char* argv[])
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

	int e{ createNewMDCServer() };
	if (eSuccess == e)
	{
		LOG(INFO) << "Create new MDC server result = " << e << ".";
	} 
	else
	{
		LOG(WARNING) << "Create new MDC server result = " << e << ".";
	}

	getchar();

	e = destroyMDCServer();
	if (eSuccess == e)
	{
		LOG(INFO) << "Destroy MDC server result = " << e << ".";
	}
	else
	{
		LOG(WARNING) << "Destroy MDC server result = " << e << ".";
	}

	google::ShutdownGoogleLogging();

    return 0;
}