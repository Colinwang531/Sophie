#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "Define.h"
#include "Error.h"
#include "Time/XTime.h"
using Time = framework::time::Time;
#include "Data/Buffer.h"
#include "Network/ASIO/Core/TCPConnector.h"
using TCPConnector = framework::network::asio::TCPConnector;
using TCPConnectorPtr = boost::shared_ptr<TCPConnector>;
#include "Network/ASIO/Session/TCPSourceSession.h"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			TCPSourceSession::TCPSourceSession(
				const std::string url,
				boost::asio::ip::tcp::socket& so, 
				AfterTCPSendResultCallback scb /* = nullptr */, 
				AfterTCPReceiveResultCallback rcb /* = nullptr */)
				: TCPSession(so, scb, rcb), streamURL{ url }
			{}
			TCPSourceSession::~TCPSourceSession()
			{}

			int TCPSourceSession::connect(
				const char* ipv4 /* = nullptr */, 
				const unsigned short port /* = 0 */)
			{
				int e{ 
					ipv4 && gMaxPortNumber > port && gMinPortNumber < port ? eSuccess : eInvalidParameter };

				if (eSuccess == e)
				{
					TCPConnectorPtr cp{
						boost::make_shared<TCPConnector>(
							boost::bind(&TCPSourceSession::afterTCPConnectResultCallback, this, _1)) };
					
					if (cp)
					{
						e = cp->connect(so, ipv4, port);
					}
				}

				return e;
			}

			void TCPSourceSession::afterTCPConnectResultCallback(const int e /* = 0 */)
			{
				//发送流URL以请求数据
				const int urlsize{ streamURL.length() };
				const int bytes{ framework::data::FixedHeadSize + urlsize };

				//直接使用基类缓存
				if (buffer)
				{
					int* flag{ (int*)buffer };
					*flag = framework::data::FixedHeadFlag;
					int* factoryType{ (int*)(buffer + 4) };
					*factoryType = static_cast<int>(framework::data::DataFactory::DATA_FACTORY_PRIVATE);
					int* dataType{ (int*)(buffer + 8) };
					*dataType = static_cast<int>(framework::data::DataType::DATA_TYPE_BYTES);
					int* frameType{ (int*)(buffer + 12) };
					*frameType = -1;
					int* frameSize{ (int*)(buffer + 16) };
					*frameSize = urlsize;
					long long* frameSequence{ (long long*)(buffer + 20) };
					*frameSequence = -1;
					long long* frameTimestamp{ (long long*)(buffer + 28) };
					*frameTimestamp = (long long)(Time().tickcount());
#ifdef WINDOWS
					memcpy_s(buffer + framework::data::FixedHeadSize, urlsize, streamURL.c_str(), urlsize);
#else
					memcpy(buffer + framework::data::FixedHeadSize, streamURL.c_str(), urlsize);
#endif//WINDOWS

					int e{ TCPSession::send(buffer, bytes) };

					if (eSuccess == e)
					{
						e = TCPSession::receive();
					}
				}
			}
		}//namespace asio
	}//namespace network
}//namespace framework
