#include "liblog/log.h"
using Log = framework::liblog::Log;
#include "libcommon/command_line/command_line.h"
using CommandLineParser = framework::libcommon::CommandLineParser;
#include "libcommon/xml/xml.h"
using XmlParser = framework::libcommon::XmlParser;
#include "libcommon/uuid/uuid.h"
using Uuid = framework::libcommon::Uuid;
#include "libnetwork/zmq/ctx.h"
using Ctx = framework::libnetwork::zmq::Ctx;
#include "libcommon/const.h"
#include "libcommon/error.h"
#include "XMQ.h"

static std::string gApplicationID;
static std::string gLocalIP;
static unsigned short gLocalPort{ 0 };

static void parseCommandLine(Log& log, int argc, char** argv)
{
	CommandLineParser parser;
	parser.setOption("local_ip", "127.0.0.1");
	parser.setOption("local_port", "61001");
	CommonError e{
		static_cast<CommonError>(
			parser.parse(argc, const_cast<const char**>(argv)))};

	if (CommonError::COMMON_ERROR_SUCCESS == e)
	{
		gLocalIP.append(parser.getValue("local_ip"));
		gLocalPort = static_cast<unsigned short>(atoi(parser.getValue("local_port")));
	}

	log.write(
		framework::liblog::LogLevel::LOG_LEVEL_INFO, 
		"Parse command line parameters with local_ip = [ %s ] and local_port = [ %d ].",
		gLocalIP.c_str(), 
		gLocalPort);
}

static void generateApplicationID(Log& log)
{
	gApplicationID.append(XmlParser().getValue("config.xml", "Component.XMQ.ID"));

	if (gApplicationID.empty())
	{
		gApplicationID.append(Uuid().generateRandomUuid());
		XmlParser().setValue("config.xml", "Component.XMQ.ID", gApplicationID);
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
		XMQ xmq{log, gApplicationID, &ctx};
		xmq.bind(gLocalIP, gLocalPort);
		getchar();
	}

	ctx.uninit();
	log.uninit();
	
    return 0;
}