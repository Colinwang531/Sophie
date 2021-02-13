#include "boost/make_shared.hpp"
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
#include "XMQ.h"

static std::string gUplinkIPv4;
static unsigned short gUplinkPort{ 0 };
static unsigned short gLocalListenPort{ 0 };

static void parseCommandLine(int argc, char** argv)
{
	CommandLine cl;
	cl.setCommandOptions("listen,l", "61001");
	cl.setCommandOptions("uplinkipv4,u", "");
	cl.setCommandOptions("uplinkport,p", "0");

	if (eSuccess == cl.parseCommandLine(argc, argv))
	{
		const char* ipv4{ cl.getParameterByOption("uplinkipv4") };
		if (ipv4)
		{
			gUplinkIPv4.append(ipv4);
		}

		const char* uplinkPort{ cl.getParameterByOption("uplinkport") };
		if (uplinkPort)
		{
			gUplinkPort = atoi(uplinkPort);
		}
		
		const char* listenPort{ cl.getParameterByOption("listen") };
		if (listenPort)
		{
			gLocalListenPort = atoi(listenPort);
		}
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
	using XMQPtr = boost::shared_ptr<framework::network::zeromq::RD>;
	XMQPtr xmq{ boost::make_shared<XMQ>() };

	if (xmq)
	{
		xmq->startRD(gLocalListenPort, gUplinkIPv4.c_str(), gUplinkPort);
	} 
	else
	{
		LOG(ERROR) << "Bad to create new XMQ object.";
	}

	getchar();

	if (xmq)
	{
		xmq->stopRD();
	}
	google::ShutdownGoogleLogging();

    return 0;
}