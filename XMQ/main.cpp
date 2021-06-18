#include <string>
#include "liblog/log.h"
using Log = framework::liblog::Log;
#include "libcommon/command_line/command_line.h"
using CommandLineParser = framework::libcommon::CommandLineParser;
#include "libcommon/path/path.h"
using Path = framework::libcommon::ProgramPathParser;
#include "XMQ.h"

static std::string gProgramName;
static std::string gUplinkAddress;
static unsigned short gUplinkPort{ 0 };
static std::string gLocalListenAddress;
static unsigned short gLocalListenPort{ 0 };

static void parseCommandLine(int argc, const char** argv)
{
	CommandLineParser clp;
	clp.setOption("address,a", "127.0.0.1");
	clp.setOption("listen,l", "61001");
	clp.setOption("uplink_address,u", "");
	clp.setOption("uplink_port,p", "0");
	clp.setOption("name,n", "unknown");

	if (0 == clp.parse(argc, argv))
	{
		const char* uplinkAddr{ clp.getValue("uplink_address") };
		if (uplinkAddr)
		{
			gUplinkAddress.append(uplinkAddr);
		}

		const char* uplinkPort{ clp.getValue("uplink_port") };
		if (uplinkPort)
		{
			gUplinkPort = atoi(uplinkPort);
		}

		const char* localAddr{ clp.getValue("address") };
		if (localAddr)
		{
			gLocalListenAddress.append(localAddr);
		}
		
		const char* listenPort{ clp.getValue("listen") };
		if (listenPort)
		{
			gLocalListenPort = atoi(listenPort);
		}

		const char* name{ clp.getValue("name") };
		if (name)
		{
			gProgramName.append(name);
		}
	}
}

int main(int argc, char* argv[])
{
	Path path;
	const std::string dirName{path.parseFileDirectory(argv[0])};
	const std::string logDirName{dirName + "/log"};
	parseCommandLine(argc, (const char**)argv);

	Log log;
	log.init(argv[0], logDirName.c_str());
	log.write(
		framework::liblog::LogLevel::LOG_LEVEL_INFO, 
		"The direction of process is [ %s ]", 
		dirName.c_str());
	log.write(
		framework::liblog::LogLevel::LOG_LEVEL_INFO, 
		"The parameters of local network for listening is [ %s : %d ]", 
		gLocalListenAddress.c_str(), gLocalListenPort);
	log.write(
		framework::liblog::LogLevel::LOG_LEVEL_INFO, 
		"The parameters of uplink network for connecting is [ %s : %d ]", 
		gUplinkAddress.c_str(), gUplinkPort);

	Dispatcher* dispatcher{
		new(std::nothrow) XMQ(log, gLocalListenAddress, gLocalListenPort, gUplinkAddress, gUplinkPort)};
	if (dispatcher)
	{
		dispatcher->start();
		getchar();
		dispatcher->stop();
	}
	else
	{
		log.write(
			framework::liblog::LogLevel::LOG_LEVEL_ERROR, 
			"Bad new object of XMQ for running.");
	}
	
    return 0;
}