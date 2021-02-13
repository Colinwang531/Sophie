#include "boost/make_shared.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
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
#include "Xml/XmlCodec.h"
using XMLParser = base::xml::XMLParser;
using XMLPacker = base::xml::XMLPacker;
#include "XMS.h"

static std::string gXMSID;
static std::string gXMSName;
static std::string gXMQIP;
static unsigned short gXMQPort{ 0 };
static unsigned short gMediaPort{ 0 };

static void parseCommandLine(int argc, char** argv)
{
	CommandLine cl;
	cl.setCommandOptions("xmq,x", "127.0.0.1");
	cl.setCommandOptions("port,p", "61001");
	cl.setCommandOptions("media,m", "60927");
	cl.setCommandOptions("name,n", "XMS");

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
		if (port)
		{
			gMediaPort = static_cast<unsigned short>(atoi(media));
		}

		//优先使用配置文件中的名称,如果配置文件中的名称空再使用参数列表中的名称
		//名称在第一次启动设置后就不能再修改
		std::string configName;
		XMLParser().getValueByName("Config.xml", "Component.XMS.Name", configName);
		const char* name{ cl.getParameterByOption("name") };
		if (configName.empty())
		{
			if (name)
			{
				configName.append(name);
				//名字直接写入配置文件,需要再去读
				XMLPacker().setValueWithName("Config.xml", "Component.XMS.Name", configName);
			}
		}
		gXMSName = configName;
	}
}

static void parseID()
{
	//优先使用配置文件中的名称,如果配置文件中的名称空再使用参数列表中的名称
		//名称在第一次启动设置后就不能再修改
	XMLParser().getValueByName("Config.xml", "Component.XMS.ID", gXMSID);
	if (gXMSID.empty())
	{
		gXMSID = boost::uuids::to_string(boost::uuids::random_generator()());
		XMLPacker().setValueWithName("Config.xml", "Component.XMS.ID", gXMSID);
	}
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
	parseID();

	using XMSPtr = boost::shared_ptr<framework::network::zeromq::OD>;
	XMSPtr xms{ boost::make_shared<XMS>(gXMSName, gXMSID, gMediaPort) };

	if (xms)
	{
		xms->startOD(gXMQIP.c_str(), gXMQPort);
	}

	getchar();

	if (xms)
	{
		xms->stopOD();
	}
	google::ShutdownGoogleLogging();
    
    return 0;
}