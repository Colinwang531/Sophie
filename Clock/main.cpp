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
#include "Error.h"
#include "CommandLine/CommandLine.h"
using CommandLine = base::commandline::CommandLine;
#include "Xml/XmlCodec.h"
using XMLParser = base::xml::XMLParser;
using XMLPacker = base::xml::XMLPacker;
#include "CLK.h"

static std::string gCLKID;
static std::string gCLKName;
static std::string gXMQIP;
static unsigned short gXMQPort{ 0 };
static int gComNumber{ 0 };
static int gComBaudrate{ 0 };

static void parseCommandLine(int argc, char** argv)
{
	CommandLine cl;
	cl.setCommandOptions("xmq,x", "127.0.0.1");
	cl.setCommandOptions("port,p", "61101");
	cl.setCommandOptions("name,n", "Clock");
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
		XMLParser().getValueByName("Config.xml", "Component.CLK.Name", configName);
		const char* name{ cl.getParameterByOption("name") };
		if (configName.empty())
		{
			if (name)
			{
				configName.append(name);
				//名字直接写入配置文件,需要再去读
				XMLPacker().setValueWithName("Config.xml", "Component.CLK.Name", configName);
			}
		}
		gCLKName = configName;
	}
}

static void parseID()
{
	//优先使用配置文件中的名称,如果配置文件中的名称空再使用参数列表中的名称
		//名称在第一次启动设置后就不能再修改
	XMLParser().getValueByName("Config.xml", "Component.CLK.ID", gCLKID);
	if (gCLKID.empty())
	{
		gCLKID = boost::uuids::to_string(boost::uuids::random_generator()());
		XMLPacker().setValueWithName("Config.xml", "Component.CLK.ID", gCLKID);
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

	using CLKPtr = boost::shared_ptr<framework::network::zeromq::OD>;
	CLKPtr clk{ boost::make_shared<CLK>(gCLKName, gCLKID, gComNumber, gComBaudrate) };

	if (clk)
	{
		clk->startOD(gXMQIP.c_str(), gXMQPort);
	}

	getchar();

	if (clk)
	{
		clk->stopOD();
	}
	google::ShutdownGoogleLogging();

	return 0;
}

// #include <errno.h>
// #include <fcntl.h> 
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <termios.h>
// #include <unistd.h>
// 
// int set_interface_attribs(int fd, int speed)
// {
// 	struct termios tty;
// 
// 	if (tcgetattr(fd, &tty) < 0) {
// 		printf("Error from tcgetattr: %s\n", strerror(errno));
// 		return -1;
// 	}
// 
// 	cfsetospeed(&tty, (speed_t)speed);
// 	cfsetispeed(&tty, (speed_t)speed);
// 
// 	tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
// 	tty.c_cflag &= ~CSIZE;
// 	tty.c_cflag |= CS8;         /* 8-bit characters */
// 	tty.c_cflag &= ~PARENB;     /* no parity bit */
// 	tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
// 	tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */
// 
// 	/* setup for non-canonical mode */
// 	tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
// 	tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
// 	tty.c_oflag &= ~OPOST;
// 
// 	/* fetch bytes as they become available */
// 	tty.c_cc[VMIN] = 1;
// 	tty.c_cc[VTIME] = 1;
// 
// 	if (tcsetattr(fd, TCSANOW, &tty) != 0) {
// 		printf("Error from tcsetattr: %s\n", strerror(errno));
// 		return -1;
// 	}
// 	return 0;
// }
// 
// void set_mincount(int fd, int mcount)
// {
// 	struct termios tty;
// 
// 	if (tcgetattr(fd, &tty) < 0) {
// 		printf("Error tcgetattr: %s\n", strerror(errno));
// 		return;
// 	}
// 
// 	tty.c_cc[VMIN] = mcount ? 1 : 0;
// 	tty.c_cc[VTIME] = 5;        /* half second timer */
// 
// 	if (tcsetattr(fd, TCSANOW, &tty) < 0)
// 		printf("Error tcsetattr: %s\n", strerror(errno));
// }
// 
// 
// int main()
// {
// 	char* portname = "/dev/ttyUSB0";
// 	int fd;
// 	int wlen;
// 
// 	fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
// 	if (fd < 0) {
// 		printf("Error opening %s: %s\n", portname, strerror(errno));
// 		return -1;
// 	}
// 	/*baudrate 115200, 8 bits, no parity, 1 stop bit */
// 	set_interface_attribs(fd, B4800);
// 	//set_mincount(fd, 0);                /* set to pure timed read */
// 
// // 	/* simple output */
// // 	wlen = write(fd, "Hello!\n", 7);
// // 	if (wlen != 7) {
// // 		printf("Error from write: %d, %d\n", wlen, errno);
// // 	}
// // 	tcdrain(fd);    /* delay for output */
// 
// 
// 	/* simple noncanonical input */
// 	do {
// 		unsigned char buf[2]{0};
// 		int rdlen;
// 
// 		rdlen = read(fd, buf, sizeof(buf) - 1);
// 		if (rdlen > 0) {
// 			printf("%s\n", buf);
// 		}
// 		else if (rdlen < 0) {
// 			printf("Error from read: %d: %s\n", rdlen, strerror(errno));
// 		}
// 		/* repeat read to get full message */
// 	} while (1);
// }