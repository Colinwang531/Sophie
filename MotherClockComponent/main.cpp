#include "boost/algorithm/string.hpp"
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
#include "CommandLine/CommandLine.h"
using CommandLine = base::commandline::CommandLine;
#include "Xml/XmlCodec.h"
using XMLParser = base::xml::XMLParser;
using XMLPacker = base::xml::XMLPacker;
#include "MotherClockComponentClient.h"
using MOtherClockComponentClientPtr = boost::shared_ptr<base::network::AbstractClient>;
#include "ComPort/ComPort.h"
using ComPort = base::com::ComPort;
using ComPortPtr_ = boost::shared_ptr<ComPort>;

static std::string gMessageDispatcherCenterIP{ "127.0.0.1" };
static unsigned short gMessageDispatcherCenterPort{ 61001 };
MOtherClockComponentClientPtr gMotherClockComponentClientPtr;
static unsigned int gComPortIndex{ 0 };
static unsigned int gComPortBaudrate{ 0 };
static std::string clockAsyncData;

static void comportDataNotificationCallback(
	const char* data = nullptr, 
	const int bytes = 0)
{
	clockAsyncData.append(data, bytes);

	if ('\n' == *data)
	{
		if (!clockAsyncData.empty())
		{
			std::vector<std::string> clockDatas;
			boost::split(clockDatas, clockAsyncData, boost::is_any_of(","));

			if (!clockDatas[0].compare("$ZQZDA") || !clockDatas[0].compare("$CJZDA") || !clockDatas[0].compare("$GPZDA"))
			{
				char clockDesc[128]{ 0 };
#ifdef WINDOWS
				sprintf_s(clockDesc, 128, "%s-%s-%s,%s:%s:%s,%s",
					clockDatas[4].c_str(), clockDatas[3].c_str(), clockDatas[2].c_str(),
					clockDatas[1].substr(0, 2).c_str(), clockDatas[1].substr(2, 2).c_str(), clockDatas[1].substr(4, 2).c_str(),
					clockDatas[5].c_str());
#else
				sprintf(clockDesc, "%s-%s-%s,%s:%s:%s,%s",
					clockDatas[4].c_str(), clockDatas[3].c_str(), clockDatas[2].c_str(),
					clockDatas[1].substr(0, 2).c_str(), clockDatas[1].substr(2, 2).c_str(), clockDatas[1].substr(4, 2).c_str(),
					clockDatas[5].c_str());
#endif//WINDOWS

				if (gMotherClockComponentClientPtr)
				{
					boost::shared_ptr<MotherClockComponentClient> mccp{ 
						boost::dynamic_pointer_cast<MotherClockComponentClient>(gMotherClockComponentClientPtr) };
					mccp->buildMotherClockMessage(clockDesc);
				}

				clockAsyncData.clear();
			}
		}
	}
}

static void parseCommandLine(int argc, char** argv)
{
	CommandLine cl;
	cl.setCommandOptions("address,a", "127.0.0.1");
	cl.setCommandOptions("port,p", "61001");
	cl.setCommandOptions("name,n", "Clock");
	cl.setCommandOptions("com,c", "0");
	cl.setCommandOptions("baudrate,b", "4800");

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

		const char* comport{ cl.getParameterByOption("com") };
		if (comport)
		{
			gComPortIndex = static_cast<unsigned int>(atoi(comport));
		}

		const char* baudrate{ cl.getParameterByOption("baudrate") };
		if (baudrate)
		{
			gComPortBaudrate = static_cast<unsigned int>(atoi(baudrate));
		}

		//����ʹ�������ļ��е�����,��������ļ��е����ƿ���ʹ�ò����б��е�����
		//�����ڵ�һ���������ú�Ͳ������޸�
		std::string configName;
		XMLParser().getValueByName("Config.xml", "Component.CLK.Name", configName);
		const char* name{ cl.getParameterByOption("name") };
		if (configName.empty())
		{
			if (name)
			{
				configName.append(name);
				//����ֱ��д�������ļ�,��Ҫ��ȥ��
				XMLPacker().setValueWithName("Config.xml", "Component.CLK.Name", configName);
			}
		}

		LOG(INFO) <<
			"Remote server address is " << gMessageDispatcherCenterIP <<
			", server port is " << gMessageDispatcherCenterPort <<
			", name is " << configName << 
			", port index " << gComPortIndex << 
			", baudrate " << gComPortBaudrate << ".";
	}
}

static int createNewAsynchronousClient(void)
{
	int e{ gMotherClockComponentClientPtr ? eObjectExisted : eSuccess };

	if (eSuccess == e)
	{
		if (!gMessageDispatcherCenterIP.empty() && gMinPortNumber < gMessageDispatcherCenterPort && gMaxPortNumber > gMessageDispatcherCenterPort)
		{
			MOtherClockComponentClientPtr ap{
				boost::make_shared<MotherClockComponentClient>() };
			if (ap)
			{
				e = ap->startClient(
					(boost::format("tcp://%s:%d") % gMessageDispatcherCenterIP % gMessageDispatcherCenterPort).str());

				if (eSuccess == e)
				{
					gMotherClockComponentClientPtr.swap(ap);
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
	int e{ gMotherClockComponentClientPtr ? eSuccess : eBadOperate };

	if (eSuccess == e)
	{
		e = gMotherClockComponentClientPtr->stopClient();

		if (eSuccess == e)
		{
			gMotherClockComponentClientPtr.reset();
		}
		else
		{
			e = eBadOperate;
		}
	}

	return e;
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

	int e{ createNewAsynchronousClient() };
	if (eSuccess == e)
	{
		LOG(INFO) << "Create new asynchronous client result = " << e << ".";
	}
	else
	{
		LOG(WARNING) << "Create new asynchronous client result = " << e << ".";
	}

	ComPort cp(
		boost::bind(&comportDataNotificationCallback, _1, _2));
	e = cp.openPort(gComPortIndex, gComPortBaudrate);
	if (eSuccess == e)
	{
		LOG(INFO) << "Open com port = " << gComPortIndex << 
			" , baudrate = " << gComPortBaudrate << 
			", result = " << e << ".";
	}
	else
	{
		LOG(WARNING) << "Open com port = " << gComPortIndex <<
			" , baudrate = " << gComPortBaudrate <<
			", result = " << e << ".";
	}

	getchar();

	e = destroyAsynchronousClient();
	if (eSuccess == e)
	{
		LOG(INFO) << "Destroy asynchronous client result = " << e << ".";
	}
	else
	{
		LOG(WARNING) << "Destroy asynchronous client result = " << e << ".";
	}

	e = cp.closePort();
	if (eSuccess == e)
	{
		LOG(INFO) << "Close com port = " << gComPortIndex <<
			" , baudrate = " << gComPortBaudrate <<
			", result = " << e << ".";
	}
	else
	{
		LOG(WARNING) << "Close com port = " << gComPortIndex <<
			" , baudrate = " << gComPortBaudrate <<
			", result = " << e << ".";
	}
    
    return 0;
}