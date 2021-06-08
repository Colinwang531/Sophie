#include "command_line/command_line.h"
using CommandLineParser = framework::libcommon::CommandLineParser;

//static std::string gUplinkIPv4;
static unsigned short gUplinkPort{ 0 };
static unsigned short gLocalListenPort{ 0 };

static void parseCommandLine(int argc, const char** argv)
{
	CommandLineParser clp;
	clp.setOption("listen,l", "61001");
	clp.setOption("uplinkipv4,u", "");
	clp.setOption("uplinkport,p", "0");

	if (0 == clp.parse(argc, argv))
	{
		/*
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
		*/
	}
}

int main(int argc, char* argv[])
{
	parseCommandLine(argc, (const char**)argv);

    return 0;
}