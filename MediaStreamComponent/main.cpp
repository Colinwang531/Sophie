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
#include "Hardware/Cpu.h"
using Cpu = base::hardware::Cpu;
#include "CommandLine/CommandLine.h"
using CommandLine = base::commandline::CommandLine;
#include "Xml/XmlCodec.h"
using XMLParser = base::xml::XMLParser;
using XMLPacker = base::xml::XMLPacker;
#include "ASIO/ASIOService.h"
using ASIOService = base::network::ASIOService;
#include "ASIO/TCPListener.h"
using TCPListener = base::network::TCPListener;
using TCPListenerPtr = boost::shared_ptr<TCPListener>;
#include "Packet/Stream/StreamPacket.h"
using StreamPacket = base::packet::StreamPacket;
#include "Session/TCPStreamSourceSession.h"
using TCPStreamSourceSession = base::network::TCPStreamSourceSession;
using TCPStreamSourceSessionPtr = boost::shared_ptr<TCPStreamSourceSession>;
#include "DataStruct/UnorderedMap.h"
using TCPStreamSourceSessionGroup = UnorderedMap<const std::string, TCPSessionPtr>;
#include "MediaStreamComponentClient.h"
using MediaStreamComponentClientPtr = boost::shared_ptr<base::network::AbstractClient>;

static TCPStreamSourceSessionGroup streamSourceSessionGroup;
static std::string gMessageDispatcherCenterIP{ "127.0.0.1" };
static unsigned short gMessageDispatcherCenterPort{ 61001 };
MediaStreamComponentClientPtr gMediaStreamComponentClientPtr;

static void parseCommandLine(int argc, char** argv)
{
	CommandLine cl;
	cl.setCommandOptions("address,a", "127.0.0.1");
	cl.setCommandOptions("port,p", "61001");
	cl.setCommandOptions("name,n", "Stream");

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

		//优先使用配置文件中的名称,如果配置文件中的名称空再使用参数列表中的名称
		//名称在第一次启动设置后就不能再修改
		std::string configName;
		XMLParser().getValueByName("Config.xml", "Component.MED.Name", configName);
		const char* name{ cl.getParameterByOption("name") };
		if (configName.empty())
		{
			if (name)
			{
				configName.append(name);
				//名字直接写入配置文件,需要再去读
				XMLPacker().setValueWithName("Config.xml", "Component.MED.Name", configName);
			}
		}

		LOG(INFO) <<
			"Remote server address is " << gMessageDispatcherCenterIP <<
			", server port is " << gMessageDispatcherCenterPort <<
			", name is " << configName << ".";
	}
}

static int createNewAsynchronousClient(void)
{
	int e{ gMediaStreamComponentClientPtr ? eObjectExisted : eSuccess };

	if (eSuccess == e)
	{
		if (!gMessageDispatcherCenterIP.empty() && gMinPortNumber < gMessageDispatcherCenterPort && gMaxPortNumber > gMessageDispatcherCenterPort)
		{
			MediaStreamComponentClientPtr mscp{ boost::make_shared<MediaStreamComponentClient>() };
			if (mscp)
			{
				e = mscp->startClient(
					(boost::format("tcp://%s:%d") % gMessageDispatcherCenterIP % gMessageDispatcherCenterPort).str().c_str());

				if (eSuccess == e)
				{
					gMediaStreamComponentClientPtr.swap(mscp);
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
	int e{ gMediaStreamComponentClientPtr ? eSuccess : eBadOperate };

	if (eSuccess == e)
	{
		e = gMediaStreamComponentClientPtr->stopClient();

		if (eSuccess == e)
		{
			gMediaStreamComponentClientPtr.reset();
		}
		else
		{
			e = eBadOperate;
		}
	}

	return e;
}

void afterGotRemoteReadDataNotificationCallback(
	boost::asio::ip::tcp::socket* s,
	const unsigned char* data, 
	const int bytes)
{
	//分析接收到的第一个数据一定是URL信息
	//确认接收的数据包含完整的数据头
	if (base::packet::StreamHeadSize::eFixedHeadSize < bytes)
	{
		//验证头标识
		const int* headFlag{ (const int*)data };
		if (base::packet::eFixedHeadFlag == *headFlag)
		{
			const int* urlbytes{ 
				(const int*)(data + 16) };
			const char* urlstr{ 
				(const char*)(data + base::packet::StreamHeadSize::eFixedHeadSize) };
			const std::string url(urlstr, *urlbytes);

			TCPSessionPtr sourceSession{ streamSourceSessionGroup.at(url) };
			//如果不存在则添加流源会话
			//如果存在则添加流目的会话
			if (sourceSession)
			{
				TCPStreamSourceSessionPtr sss{ 
					boost::dynamic_pointer_cast<TCPStreamSourceSession>(sourceSession) };
				sss->addStreamTargetSession(s);

				LOG(INFO) << "Add new target stream session = " << url << ".";
			} 
			else
			{
				sourceSession = boost::make_shared<TCPStreamSourceSession>(s);
				if (sourceSession && eSuccess == sourceSession->startSession())
				{
					streamSourceSessionGroup.insert(url, sourceSession);
					LOG(INFO) << "Add new source stream session = " << url << ".";
				}
				else
				{
					LOG(WARNING) << "Bad new source stream session = " << url << ".";
				}
			}
		}
	}
}

void afterGotRemoteConnectedNotificationCallback(boost::asio::ip::tcp::socket* s, boost::system::error_code e)
{
	const std::string remoteIP{ s->remote_endpoint().address().to_string() };
	const unsigned short remotePort{ s->remote_endpoint().port() };
	LOG(INFO) << "Remote [ " << remoteIP << ":" << remotePort << " ] is connected, result = " << e.value() << ".";

	TCPReceiverPtr receiver{ 
		boost::make_shared<TCPReceiver>(
			boost::bind(&afterGotRemoteReadDataNotificationCallback, _1, _2, _3)) };

	if (receiver)
	{
		receiver->recvData(s);
	}
}

void postTCPListener(ASIOService& asio)
{
	for (int i = 0; i != 1/*Cpu().getCPUCoreNumber()*/; ++i)
	{
		TCPListenerPtr listener{ 
			boost::make_shared<TCPListener>(
				asio,
				boost::bind(&afterGotRemoteConnectedNotificationCallback, _1, _2)) };

		if (listener)
		{
			int e{ listener->setListen() };

			if (eSuccess == e)
			{
				LOG(INFO) << "Create new TCP listener" << " [ " << i << " ] " << "sucessfully.";
			} 
			else
			{
				LOG(INFO) << "Create new TCP listener" << " [ " << i << " ] " << "failed result = " << e << ".";
			}
		} 
		else
		{
			LOG(WARNING) << "Create new TCP listener object" << " [ " << i << " ] " << "failed.";
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

	ASIOService asio;
	int e{ createNewAsynchronousClient() };
	if (eSuccess == e)
	{
		LOG(INFO) << "Create new asynchronous client result = " << e << ".";
		
		int e{ asio.startService() };
		if (eSuccess == e)
		{
			LOG(INFO) << "Start media stream server sucessfully.";
			postTCPListener(asio);
		}
		else
		{
			LOG(WARNING) << "Start media stream server fail result = " << e << ".";
		}
	}
	else
	{
		LOG(WARNING) << "Create new asynchronous client result = " << e << ".";
	}

	getchar();

	asio.stopService();
	LOG(INFO) << "Stop media stream server.";
	e = destroyAsynchronousClient();
	if (eSuccess == e)
	{
		LOG(INFO) << "Destroy asynchronous client result = " << e << ".";
	}
	else
	{
		LOG(WARNING) << "Destroy asynchronous client result = " << e << ".";
	}
    
    return 0;
}