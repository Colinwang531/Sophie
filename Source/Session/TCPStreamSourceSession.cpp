#include "boost/make_shared.hpp"
#include "Error.h"
#include "Packet/Stream/StreamPacket.h"
using StreamPacket = base::packet::StreamPacket;
#include "Session/TCPStreamTargetSession.h"
#include "Session/TCPStreamSourceSession.h"

namespace base
{
	namespace network
	{
		TCPStreamSourceSession::TCPStreamSourceSession(
			boost::asio::ip::tcp::socket* s /* = nullptr */) 
			: TCPSession(s)
		{}

		TCPStreamSourceSession::~TCPStreamSourceSession()
		{}

		int TCPStreamSourceSession::addStreamTargetSession(
			boost::asio::ip::tcp::socket* s /* = nullptr */)
		{
			int e{ s && s->is_open() ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				TCPSessionPtr session{ 
					boost::make_shared<TCPStreamTargetSession>(s) };

				if (session && eSuccess == session->startSession())
				{
					streamTargetSessionGroup.pushBack(session);
				}
				else
				{
					e = eBadNewObject;
				}
			}

			return e;
		}

		void TCPStreamSourceSession::receivedDataNotification(
			const unsigned char* data /* = nullptr */, 
			const int bytes /* = 0 */)
		{
			int sessionNumber{ streamTargetSessionGroup.size() }, idx = 0;

			while (idx < sessionNumber)
			{
				TCPSessionPtr target{ streamTargetSessionGroup.at(idx++) };
				if (target->valid())
				{
					//转发数据的包不需要设置包类型和帧类型
					//只保存原始数据记录
					StreamPacket* pkt{ new(std::nothrow) StreamPacket() };
					if (pkt)
					{
						pkt->setStreamData(data, bytes);
						target->sendData(pkt);
					}
				}
			}

			//数据处理完成之后才能再次接收
			//否则会导致数据乱序的问题
			//但是否可以考虑将数据缓存在每次接收时动态创建以解决这个问题
			TCPSession::receivedDataNotification(data, bytes);
		}

		void TCPStreamSourceSession::receivedExceptionNotification(const boost::system::error_code e)
		{
			TCPSession::receivedExceptionNotification(e);
			streamTargetSessionGroup.clear();
		}

		void TCPStreamSourceSession::receivedExpiredNotification()
		{
			TCPSession::receivedExpiredNotification();
			streamTargetSessionGroup.clear();
		}
	}//namespace network
}//namespace base
