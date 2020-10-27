#include "boost/algorithm/string.hpp"
#include "boost/bind.hpp"
#include "boost/format.hpp"
#include "boost/make_shared.hpp"
#include "boost/pointer_cast.hpp"
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
#include "AISPusherComponentClient.h"
using AISPusherComponentClientPtr = boost::shared_ptr<base::network::AbstractClient>;
#include "ComPort/ComPort.h"
using ComPort = base::com::ComPort;
using ComPortPtr_ = boost::shared_ptr<ComPort>;

static std::string gMessageDispatcherCenterIP{ "127.0.0.1" };
static unsigned short gMessageDispatcherCenterPort{ 61001 };
AISPusherComponentClientPtr gAISPusherComponentClientPtr;
static unsigned int gComPortIndex{ 0 };
static unsigned int gComPortBaudrate{ 0 };
static std::string aisAsyncData;

static void comportDataNotificationCallback(
	const char* data = nullptr,
	const int bytes = 0)
{
	aisAsyncData.append(data, bytes);

	if ('\n' == *data)
	{
		if (!aisAsyncData.empty())
		{
			std::vector<std::string> aisinfoDatas;
			boost::split(aisinfoDatas, aisAsyncData, boost::is_any_of(","));
			const std::string flags{ aisinfoDatas[0] };

			if ('$' == flags[0] || 'V' == flags[3] || 'S' == flags[4] || 'D' == flags[5])
			{
				int sailStatus{ atoi(aisinfoDatas[8].c_str()) };

				if (gAISPusherComponentClientPtr)
				{
					boost::shared_ptr<AISPusherComponentClient> aisp{
						boost::dynamic_pointer_cast<AISPusherComponentClient>(gAISPusherComponentClientPtr) };
					aisp->buildAISInfoMessage(sailStatus);
				}

				aisAsyncData.clear();
			}
		}
	}
}

static void parseCommandLine(int argc, char** argv)
{
	CommandLine cl;
	cl.setCommandOptions("address,a", "127.0.0.1");
	cl.setCommandOptions("port,p", "61001");
	cl.setCommandOptions("name,n", "AIS");
	cl.setCommandOptions("com,c", "1");
	cl.setCommandOptions("baudrate,b", "38400");

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

		const char* comport{ cl.getParameterByOption("com") };
		if (comport)
		{
			gComPortIndex = static_cast<unsigned int>(atoi(comport));
		}

		const char* baudrate{ cl.getParameterByOption("baudrate") };
		if (baudrate)
		{
			gComPortBaudrate = static_cast<unsigned int>(atoi(baudrate));
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
				XMLPacker().setValueWithName("Config.xml", "Component.AIS.Name", name);
			}
		}

		LOG(INFO) <<
			"Remote server address is " << gMessageDispatcherCenterIP <<
			", server port is " << gMessageDispatcherCenterPort <<
			", name is " << configName <<
			", port index " << gComPortIndex <<
			", baudrate " << gComPortBaudrate << ".";
	}
}

static int createNewAsynchronousClient(void)
{
	int e{ gAISPusherComponentClientPtr ? eObjectExisted : eSuccess };

	if (eSuccess == e)
	{
		if (!gMessageDispatcherCenterIP.empty() && gMinPortNumber < gMessageDispatcherCenterPort && gMaxPortNumber > gMessageDispatcherCenterPort)
		{
			AISPusherComponentClientPtr ap{ boost::make_shared<AISPusherComponentClient>() };
			if (ap)
			{
				e = ap->startClient(
					(boost::format("tcp://%s:%d") % gMessageDispatcherCenterIP % gMessageDispatcherCenterPort).str());

				if (eSuccess == e)
				{
					gAISPusherComponentClientPtr.swap(ap);
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
	int e{ gAISPusherComponentClientPtr ? eSuccess : eBadOperate };

	if (eSuccess == e)
	{
		e = gAISPusherComponentClientPtr->stopClient();

		if (eSuccess == e)
		{
			gAISPusherComponentClientPtr.reset();
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

	int e{ createNewAsynchronousClient() };
	if (eSuccess == e)
	{
		LOG(INFO) << "Create new asynchronous client result = " << e << ".";
	}
	else
	{
		LOG(WARNING) << "Create new asynchronous client result = " << e << ".";
	}

	ComPort cp(
		boost::bind(&comportDataNotificationCallback, _1, _2));
	e = cp.openPort(gComPortIndex, gComPortBaudrate);
	if (eSuccess == e)
	{
		LOG(INFO) << "Open com port = " << gComPortIndex <<
			" , baudrate = " << gComPortBaudrate <<
			", result = " << e << ".";
	}
	else
	{
		LOG(WARNING) << "Open com port = " << gComPortIndex <<
			" , baudrate = " << gComPortBaudrate <<
			", result = " << e << ".";
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

	e = cp.closePort();
	if (eSuccess == e)
	{
		LOG(INFO) << "Close com port = " << gComPortIndex <<
			" , baudrate = " << gComPortBaudrate <<
			", result = " << e << ".";
	}
	else
	{
		LOG(WARNING) << "Close com port = " << gComPortIndex <<
			" , baudrate = " << gComPortBaudrate <<
			", result = " << e << ".";
	}
    
    return 0;
}