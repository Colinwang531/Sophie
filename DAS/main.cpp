#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "liblog/log.h"
using Log = framework::liblog::Log;
#include "libcommon/path/path.h"
using ProgramPathParser = framework::libcommon::ProgramPathParser;

static unsigned short gDASUDPListenPortNumber{ 0 };

static void parseCommandLine(int argc, char** argv)
{
	const char* p2{ argv[1] };
	const char* p3{ argv[2] };

	if (!strncmp("-l", p2, 2))
	{
		gDASUDPListenPortNumber = static_cast<unsigned short>(atoi(p3));
	}
}

int main(int argc, char* argv[])
{
//	const std::string dir{ProgramPathParser().parseFileDirectory(argv[0])};
//	const std::string file{ProgramPathParser().parseFileName(argv[0])};

	Log log;
	log.init(argv[0], "");
	log.write(framework::liblog::LogLevel::LOG_LEVEL_INFO, "What is this ?");
	log.write(framework::liblog::LogLevel::LOG_LEVEL_WARNING, "What is this ?");
	log.write(framework::liblog::LogLevel::LOG_LEVEL_ERROR, "What is this ?");
	
	return -1;
	/*
	parseCommandLine(argc, argv);

	int fd{ socket(AF_INET, SOCK_DGRAM, 0) };
	if (0 > fd)
	{
		return -1;
	}

	struct sockaddr_in local;
	int len;
	memset(&local, 0, sizeof(struct sockaddr_in));
	local.sin_family = AF_INET;
	local.sin_port = htons(61001);
	local.sin_addr.s_addr = htonl(INADDR_ANY);  //�Զ���ȡIP��ַ
	len = sizeof(local);

	if (0 > bind(fd, (struct sockaddr*)&local, sizeof(local)))
	{
		return -2;
	}

	char buffer[100]{ 0 };
	struct sockaddr_in remote;

	while (1)
	{
		int bytes{ 
			static_cast<int>(recvfrom(fd, buffer, 100, 0, (struct sockaddr*)&remote, (socklen_t*)&len)) };

		if (0 > bytes)
		{
			continue;
		}

		struct sockaddr_in* sockin{ (struct sockaddr_in*)&remote };
		int port{ ntohs(sockin->sin_port) };
#ifdef WINDOWS
//		printf("ip:port  %s : %d", inet_ntoa(sock->sin_addr), port);
#else
		struct in_addr in { sockin->sin_addr };
		char str[INET_ADDRSTRLEN]{0};
		inet_ntop(AF_INET, &in, str, sizeof(str));
		printf("Data from [ %s : %d ].\r\n", str, port);
#endif
		
		sendto(fd, str, strlen(str), 0, (struct sockaddr*)&remote, len);
	}

	return 0;
	*/
}