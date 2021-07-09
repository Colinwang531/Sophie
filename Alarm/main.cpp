#include "libcommon/command_line/command_line.h"
using CommandLineParser = framework::libcommon::CommandLineParser;
#include "libcommon/xml/xml.h"
using XmlParser = framework::libcommon::XmlParser;
#include "libcommon/uuid/uuid.h"
using Uuid = framework::libcommon::Uuid;
#include "libnetwork/zmq/ctx.h"
using Ctx = framework::libnetwork::zmq::Ctx;
#include "libcommon/error.h"
#include "alarm.h"

static std::string gApplicationID;
static std::string gRemoteIP;
static unsigned short gRemotePort{ 0 };
static std::string gPublisherIP;
static unsigned short gPublisherPort{ 0 };

static void parseCommandLine(Log& log, int argc, char** argv)
{
	CommandLineParser parser;
	parser.setOption("remote_ip", "127.0.0.1");
	parser.setOption("remote_port,p", "61001");
	parser.setOption("publisher_ip", "127.0.0.1");
	parser.setOption("publisher_port", "60820");
	CommonError e{
		static_cast<CommonError>(
			parser.parse(argc, const_cast<const char**>(argv)))};

	if (CommonError::COMMON_ERROR_SUCCESS == e)
	{
		gRemoteIP.append(parser.getValue("remote_ip"));
		gRemotePort = static_cast<unsigned short>(atoi(parser.getValue("remote_port")));
		gPublisherIP.append(parser.getValue("publisher_ip"));
		gPublisherPort = static_cast<unsigned short>(atoi(parser.getValue("publisher_port")));
	}

	log.write(
		framework::liblog::LogLevel::LOG_LEVEL_INFO, 
		"Parse command line parameters with remote = [ %s : %d ] and publisher = [ %s : %d ].",
		gRemoteIP.c_str(),
		gRemotePort,
		gPublisherIP.c_str(),
		gPublisherPort);
}

static void generateApplicationID(Log& log)
{
	gApplicationID.append(XmlParser().getValue("config.xml", "Component.Alarm.ID"));

	if (gApplicationID.empty())
	{
		gApplicationID.append(Uuid().generateRandomUuid());
		XmlParser().setValue("config.xml", "Component.Alarm.ID", gApplicationID);
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
		Alarm alarm{log, gApplicationID, &ctx};
		alarm.start(gPublisherIP, gPublisherPort, gRemoteIP, gRemotePort);
		getchar();
		alarm.stop();
	}

	ctx.uninit();
	log.uninit();
    
    return 0;
}