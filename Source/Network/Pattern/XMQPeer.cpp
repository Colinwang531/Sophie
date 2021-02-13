#include "boost/bind.hpp"
#include "boost/format.hpp"
#include "boost/shared_ptr.hpp"
#ifdef WINDOWS
#include "glog/log_severity.h"
#include "glog/logging.h"
#else
#include "glog/log_severity.h"
#include "glog/logging.h"
#endif//WINDOWS
#include "Define.h"
#include "Error.h"
#include "Time/XTime.h"
using Time = framework::time::Time;
#include "Thread/ThreadPool.h"
using ThreadPool = framework::thread::ThreadPool;
#include "Network/ZMQ/Msg_.h"
using Msg = framework::network::zeromq::Msg;
#include "Network/ASIO/Session/UDPSession.h"
using UDPSession = framework::network::asio::UDPSession;
using UDPSessionPtr = boost::shared_ptr<UDPSession>;
#include "Network/Pattern/XMQPeer.h"

namespace framework
{
	namespace network
	{
		namespace pattern
		{
			XMQPeer::XMQPeer(
				const std::string name,
				const std::string id)
				: OD(), ASIO(), /*UDPSession(),*/ isInit{ false }, xmqPort{ 0 }, registerSequenceNumber{ 0 }, peerName{ name }, peerID{ id }
			{}
			XMQPeer::~XMQPeer()
			{}

			int XMQPeer::startOD(
				const char* ipv4 /* = nullptr */,
				const unsigned short port /* = 0 */)
			{
				int e{ ASIO::startASIO() };

				if (eSuccess == e)
				{
					xmqAddr.append(ipv4);
					xmqPort = port;
					ThreadPool::get_mutable_instance().createNewThread(
						boost::bind(&XMQPeer::queryLocalIPv4Thread, this, ipv4, port));
				}

				return e;
			}

			int XMQPeer::stopOD()
			{
				int e{ ASIO::stopASIO() };

				if (eSuccess == e)
				{
					e = OD::stopOD();
				}

				return e;
			}

			void XMQPeer::afterUDPSendResultNotification(
				const int e /* = 0 */, 
				const int bytes /* = 0 */)
			{}

			void XMQPeer::afterUDPReceiveResultNotification(
				const int e /* = 0 */, 
				const void* data /* = nullptr */, 
				const int bytes /* = 0 */, 
				const char* ipv4 /* = nullptr */, 
				const unsigned short port /* = 0 */)
			{
				if (!e)
				{
					const char* remoteIPv4{ reinterpret_cast<const char*>(data) };
					localAddr = remoteIPv4;
					LOG(INFO) << peerName << " service parse local IP address message result = [ " << localAddr << " ].";

					if (!isInit)
					{
						if (eSuccess == OD::startOD(xmqAddr.c_str(), xmqPort))
						{
							isInit = true;
							ThreadPool::get_mutable_instance().createNewThread(
								boost::bind(&XMQPeer::sendRegisterMessageThread, this));
						}
					}
				}
				else
				{
					LOG(WARNING) << peerName << " service parse local IP address message failed, result = " << e << ".";
				}
			}

// 			void XMQPeer::afterUDPSessionExceptionNotification(const int e /* = 0 */)
// 			{}

			void XMQPeer::queryLocalIPv4Thread(
				const char* ipv4 /* = nullptr */, 
				const unsigned short port /* = 0 */)
			{
				UDPSessionPtr sp{ 
					boost::make_shared<UDPSession>(
						ASIO::getUDPSocket(),
						boost::bind(&XMQPeer::afterUDPSendResultNotification, this, _1, _2),
						boost::bind(&XMQPeer::afterUDPReceiveResultNotification, this, _1, _2, _3, _4, _5))};
				unsigned long long lastTickcount{ 0 };

				while (!OD::isStopped() && sp)
				{
					unsigned long long nowTickcount{ Time().tickcount() };

					if (30000 < nowTickcount - lastTickcount)
					{
						int e{
							sp->send(gAddressCommandName, static_cast<int>(strlen(gAddressCommandName)), ipv4, port) };

						if (eSuccess == e)
						{
							LOG(INFO) << peerName << " service send local IP address message successfully.";
							sp->receive();
						}

						lastTickcount = nowTickcount;
					}

					boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
				}
			}

			void XMQPeer::sendRegisterMessageThread()
			{
				unsigned long long lastTickcount{ 0 };

				while (!OD::isStopped())
				{
					const unsigned long long currentTickcount{ Time().tickcount() };

					if (30000 < currentTickcount - lastTickcount)
					{
						//-------------------------------------------------------------------------------------
						//| "" | "worker" | "dhs" | "cms" | "" | sequence | timestamp | "component" | message |
						//-------------------------------------------------------------------------------------
						Msg msg;
						msg.addMessage(buildRegisterMessage());
						msg.addMessage(gComponentCommandName);
						msg.addMessage((boost::format("%d") % (int)Time().tickcount()).str());
						msg.addMessage((boost::format("%d") % ++registerSequenceNumber).str());
						msg.addMessage(gEmptyData);
						msg.addMessage(gCMSComponentName);
						msg.addMessage(peerName/*gDHSComponentName*/);
						msg.addMessage(gWorkerModuleName);
						msg.addMessage(gEmptyData);

						int e{ OD::sendMsg(&msg) };
						if (eSuccess == e)
						{
							LOG(INFO) << peerName << " send message for registerring successfully.";
						}
						else
						{
							LOG(WARNING) << peerName << " send message for registerring failed, result = " << e << ".";
						}

						lastTickcount = currentTickcount;
					}

					boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
				}
			}
		}//namespace pattern
	}//namespace network
}//namespace framework
