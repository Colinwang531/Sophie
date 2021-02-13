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
#include "AIS.h"

static std::string gAISID;
static std::string gAISName;
static std::string gXMQIP;
static unsigned short gXMQPort{ 0 };
static int gComNumber{ 0 };
static int gComBaudrate{ 0 };

static void parseCommandLine(int argc, char** argv)
{
	CommandLine cl;
	cl.setCommandOptions("xmq,x", "127.0.0.1");
	cl.setCommandOptions("port,p", "61101");
	cl.setCommandOptions("name,n", "AIS");
	cl.setCommandOptions("com,c", "1");
	cl.setCommandOptions("baudrate,b", "38400");

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

		const char* comport{ cl.getParameterByOption("com") };
		if (comport)
		{
			gComNumber = atoi(comport);
		}

		const char* baudrate{ cl.getParameterByOption("baudrate") };
		if (baudrate)
		{
			gComBaudrate = atoi(baudrate);
		}

		//优先使用配置文件中的名称,如果配置文件中的名称空再使用参数列表中的名称
		//名称在第一次启动设置后就不能再修改
		std::string configName;
		XMLParser().getValueByName("Config.xml", "Component.AIS.Name", configName);
		const char* name{ cl.getParameterByOption("name") };
		if (configName.empty())
		{
			if (name)
			{
				configName.append(name);
				//名字直接写入配置文件,需要再去读
				XMLPacker().setValueWithName("Config.xml", "Component.AIS.Name", configName);
			}
		}
		gAISName = configName;
	}
}

static void parseID()
{
	//优先使用配置文件中的名称,如果配置文件中的名称空再使用参数列表中的名称
		//名称在第一次启动设置后就不能再修改
	XMLParser().getValueByName("Config.xml", "Component.AIS.ID", gAISID);
	if (gAISID.empty())
	{
		gAISID = boost::uuids::to_string(boost::uuids::random_generator()());
		XMLPacker().setValueWithName("Config.xml", "Component.AIS.ID", gAISID);
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

	using AISPtr = boost::shared_ptr<framework::network::zeromq::OD>;
	AISPtr ais{ boost::make_shared<AIS>(gAISName, gAISID, gComNumber, gComBaudrate) };

	if (ais)
	{
		ais->startOD(gXMQIP.c_str(), gXMQPort);
	}

	getchar();

	if (ais)
	{
		ais->stopOD();
	}
    
    return 0;
}