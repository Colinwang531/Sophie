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
#include "Helmet.h"

static std::string gHelmetID;
static std::string gHelmetName;
static std::string gXMQIP;
static unsigned short gXMQPort{ 0 };

static void parseCommandLine(int argc, char** argv)
{
	CommandLine cl;
	cl.setCommandOptions("xmq,x", "127.0.0.1");
	cl.setCommandOptions("port,p", "61001");
	cl.setCommandOptions("name,n", "Helmet");

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

		//����ʹ�������ļ��е�����,��������ļ��е����ƿ���ʹ�ò����б��е�����
		//�����ڵ�һ���������ú�Ͳ������޸�
		std::string configName;
		XMLParser().getValueByName("Config.xml", "Component.Helmet.Name", configName);
		const char* name{ cl.getParameterByOption("name") };
		if (configName.empty())
		{
			if (name)
			{
				configName.append(name);
				//����ֱ��д�������ļ�,��Ҫ��ȥ��
				XMLPacker().setValueWithName("Config.xml", "Component.Helmet.Name", configName);
			}
		}
		gHelmetName = configName;
	}
}

static void parseID()
{
	//����ʹ�������ļ��е�����,��������ļ��е����ƿ���ʹ�ò����б��е�����
		//�����ڵ�һ���������ú�Ͳ������޸�
	XMLParser().getValueByName("Config.xml", "Component.Helmet.ID", gHelmetID);
	if (gHelmetID.empty())
	{
		gHelmetID = boost::uuids::to_string(boost::uuids::random_generator()());
		XMLPacker().setValueWithName("Config.xml", "Component.Helmet.ID", gHelmetID);
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

	using HelmetPtr = boost::shared_ptr<framework::network::zeromq::OD>;
	HelmetPtr helmet{ boost::make_shared<Helmet>(gHelmetName, gHelmetID) };

	if (helmet)
	{
		helmet->startOD(gXMQIP.c_str(), gXMQPort);
	}

	getchar();

	if (helmet)
	{
		helmet->stopOD();
	}
	google::ShutdownGoogleLogging();
    
    return 0;
}