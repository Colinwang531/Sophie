#include "libcommon/command_line/command_line.h"
using CommandLineParser = framework::libcommon::CommandLineParser;
#include "libcommon/xml/xml.h"
using XmlParser = framework::libcommon::XmlParser;
#include "libcommon/uuid/uuid.h"
using Uuid = framework::libcommon::Uuid;
#include "libnetwork/zmq/ctx.h"
using Ctx = framework::libnetwork::zmq::Ctx;
#include "libcommon/error.h"
#include "ais.h"

static std::string gApplicationID;
static std::string gRemoteIP;
static unsigned short gRemotePort{ 0 };
static int gComNumber{ -1 };
static int gComBaudrate{ 0 };

static void parseCommandLine(Log& log, int argc, char** argv)
{
	CommandLineParser parser;
	parser.setOption("remote_ip", "127.0.0.1");
	parser.setOption("remote_port,p", "61001");
	parser.setOption("number", "0");
	parser.setOption("baudrate", "0");
	CommonError e{
		static_cast<CommonError>(
			parser.parse(argc, const_cast<const char**>(argv)))};

	if (CommonError::COMMON_ERROR_SUCCESS == e)
	{
		gRemoteIP.append(parser.getValue("remote_ip"));
		gRemotePort = static_cast<unsigned short>(atoi(parser.getValue("remote_port")));
		gComNumber = static_cast<int>(atoi(parser.getValue("number")));
		gComBaudrate = static_cast<int>(atoi(parser.getValue("baudrate")));
	}

	log.write(
		framework::liblog::LogLevel::LOG_LEVEL_INFO, 
		"Parse command line parameters with remote = [ %s : %d ] and com = [ %d : %d ].",
		gRemoteIP.c_str(),
		gRemotePort,
		gComNumber,
		gComBaudrate);
}

static void generateApplicationID(Log& log)
{
	gApplicationID.append(XmlParser().getValue("config.xml", "Component.AIS.ID"));

	if (gApplicationID.empty())
	{
		gApplicationID.append(Uuid().generateRandomUuid());
		XmlParser().setValue("config.xml", "Component.AIS.ID", gApplicationID);
	}

	log.write(
		framework::liblog::LogLevel::LOG_LEVEL_INFO, 
		"Generate application ID = [ %s ].",
		gApplicationID.c_str());
}

int main(int argc, char* argv[])
{
	Log log;
	int logErr{log.init(argv[0])};
	parseCommandLine(log, argc, argv);
	generateApplicationID(log);
	
	Ctx ctx;

	if (CommonError::COMMON_ERROR_SUCCESS == static_cast<CommonError>(ctx.init()))
	{
		AIS ais{log, gApplicationID, &ctx};
		ais.start(gRemoteIP, gRemotePort, gComNumber, gComBaudrate);
		getchar();
		ais.stop();
	}

	ctx.uninit();
	log.uninit();

	return 0;
}