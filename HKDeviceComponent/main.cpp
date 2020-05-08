#ifdef _WINDOWS
#include "glog/log_severity.h"
#include "glog/logging.h"
#else
#include "glog/log_severity.h"
#include "glog/logging.h"
#endif//_WINDOWS
#include "Error.h"
#include "CommandLine/CommandLine.h"
using CommandLine = base::commandline::CommandLine;
#include "Device/Hikvision/HikvisionDevice.h"
using Device = base::device::AbstractDevice;

int main(int argc, char** argv)
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

	LOG(INFO) << "Hello glog ...";

	CommandLine cl;
	cl.setCommandOptions("address,a", "127.0.0.1");
	cl.setCommandOptions("listen,l", "610001");
	cl.setCommandOptions("name,n", "admin");
	cl.setCommandOptions("password,p", "admin");
	cl.parseCommandLine(argc, argv);
	const char* address{ cl.getParameterByOption("address") };
	const char* port{ cl.getParameterByOption("listen") };

	Device* device{ new base::device::HikvisionDevice("admin", "eaton12345", "192.168.0.200") };
	if (device)
	{
		if (eSuccess == device->createNewDevice())
		{
			device->getDeviceConfig();
		}
	}
	getchar();
	device->destoryDevice();

	google::ShutdownGoogleLogging();
    return 0;
}