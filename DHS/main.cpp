#include "boost/make_shared.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#ifdef WINDOWS
#include "glog/log_severity.h"
#include "glog/logging.h"
#else
#include "glog/log_severity.h"
#include "glog/logging.h"
#endif//WINDOWS
#include "Error.h"
#include "CommandLine/CommandLine.h"
using CommandLine = base::commandline::CommandLine;
#include "Xml/XmlCodec.h"
using XMLParser = base::xml::XMLParser;
using XMLPacker = base::xml::XMLPacker;
#include "DHS.h"
using DHSPtr = boost::shared_ptr<DHS>;

static std::string gDHSID;
static std::string gDHSName;
static std::string gXMQIP;
static unsigned short gXMQPort{ 0 };
static unsigned short gMediaPort{ 0 };
static DHSPtr gDHSPtr;

static void parseCommandLine(int argc, char** argv)
{
	CommandLine cl;
	cl.setCommandOptions("xmq,x", "127.0.0.1");
	cl.setCommandOptions("port,p", "61001");
	cl.setCommandOptions("media,m", "65310");
	cl.setCommandOptions("name,n", "DHS");

	if (eSuccess == cl.parseCommandLine(argc, argv))
	{
		const char* ipv4{ cl.getParameterByOption("xmq") };
		if (ipv4)
		{
			gXMQIP.append(ipv4);
		}

		const char* port{ cl.getParameterByOption("port") };
		if (port)
		{
			gXMQPort = static_cast<unsigned short>(atoi(port));
		}

		const char* media{ cl.getParameterByOption("media") };
		if (media)
		{
			gMediaPort = static_cast<unsigned short>(atoi(media));
		}

		//优先使用配置文件中的名称,如果配置文件中的名称空再使用参数列表中的名称
		//名称在第一次启动设置后就不能再修改
		std::string configName;
		XMLParser().getValueByName("Config.xml", "Component.DHS.Name", configName);
		const char* name{ cl.getParameterByOption("name") };
		if (configName.empty())
		{
			if (name)
			{
				configName.append(name);
				//名字直接写入配置文件,需要再去读
				XMLPacker().setValueWithName("Config.xml", "Component.DHS.Name", configName);
			}
		}
		gDHSName = configName;
	}
}

static void parseID()
{
	//优先使用配置文件中的名称,如果配置文件中的名称空再使用参数列表中的名称
		//名称在第一次启动设置后就不能再修改
	XMLParser().getValueByName("Config.xml", "Component.DHS.ID", gDHSID);
	if (gDHSID.empty())
	{
		gDHSID = boost::uuids::to_string(boost::uuids::random_generator()());
		XMLPacker().setValueWithName("Config.xml", "Component.DHS.ID", gDHSID);
	}
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
	parseID();

	int e{ eBadNewObject };
	DHSPtr dhs{ boost::make_shared<DHS>(gDHSName, gDHSID, gMediaPort) };

	if (dhs)
	{
		e = dhs->startOD(gXMQIP.c_str(), gXMQPort);
		if (eSuccess == e)
		{
			gDHSPtr.swap(dhs);
		}
	}

	getchar();

	if (eSuccess == e)
	{
		gDHSPtr->stopOD();
	}
	google::ShutdownGoogleLogging();

    return 0;
}