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
					//ת�����ݵİ�����Ҫ���ð����ͺ�֡����
					//ֻ����ԭʼ���ݼ�¼
					StreamPacket* pkt{ new(std::nothrow) StreamPacket() };
					if (pkt)
					{
						pkt->setStreamData(data, bytes);
						target->sendData(pkt);
					}
				}
			}

			//���ݴ������֮������ٴν���
			//����ᵼ���������������
			//���Ƿ���Կ��ǽ����ݻ�����ÿ�ν���ʱ��̬�����Խ���������
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
