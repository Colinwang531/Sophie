#include "boost/bind.hpp"
#include "boost/checked_delete.hpp"
#include "boost/make_shared.hpp"
#include "boost/pointer_cast.hpp"
#include "Error.h"
#include "ASIO/TCPSession.h"

namespace base
{
	namespace network
	{
		TCPSession::TCPSession(
			boost::asio::ip::tcp::socket* s /* = nullptr */)
			: so{ s }, stopped{ false }
		{}

		TCPSession::~TCPSession()
		{
			stopSession();
		}

		int TCPSession::startSession()
		{
			int e{ so ? eSuccess : eBadOperate };

			if (eSuccess == e)
			{
				TCPSenderPtr sender{ boost::make_shared<TCPSender>(this) };
				TCPReceiverPtr receiver{ boost::make_shared<TCPReceiver>(this) };

				if (sender && receiver)
				{
					senderPtr.swap(sender);
					receiverPtr.swap(receiver);
					receiverPtr->recvData(so);
				}
				else
				{
					e = eBadNewObject;
				}
			}

			return e;
		}

		int TCPSession::stopSession()
		{
			int e{ so ? eSuccess : eBadOperate };

			if (eSuccess == e)
			{
				stopped = true;
				//SOCKET句柄是会话实例的属性
				//它只能在会话实例中被关闭和销毁
				//发送器和接收器只能是使用它而已
				so->close();
				boost::checked_delete(so);
				so = nullptr;

// 				while(0 < streamPacketGroup.size())
// 				{
// 					StreamPacket* sp{ streamPacketGroup.front() };
// 					boost::checked_delete(sp);
// 					streamPacketGroup.popFront();
// 				}
			}

			return e;
		}

		int TCPSession::sendData(
			StreamPacket* pkt /* = nullptr */)
		{
			int e{ pkt ? (senderPtr ? eSuccess : eBadOperate) : eInvalidParameter };

			if (eSuccess == e)
			{
// 				const bool empty{ 0 == streamPacketGroup.size() };
// 				streamPacketGroup.pushBack(pkt);
// 
// 				if (empty)
// 				{
// 					senderPtr->sendData(so, pkt->getStreamData(), pkt->getStreamDataBytes());
// 				}

				senderPtr->sendData(so, (const unsigned char*)(pkt->getStreamData()), pkt->getStreamDataBytes());
				boost::checked_delete(pkt);
			}

			return e;
		}

		void TCPSession::sentdDataNotification(
			const boost::system::error_code e,
			const int bytes /* = 0 */)
		{
// 			if (!e)
// 			{
// 				//删除已发送数据
// 				StreamPacket* sp{ streamPacketGroup.front() };
// 				boost::checked_delete(sp);
// 				streamPacketGroup.popFront();
// 				//发送下一帧数据
// 				if (0 < streamPacketGroup.size() && senderPtr)
// 				{
// 					StreamPacket* pkt{ streamPacketGroup.front() };
// 					senderPtr->sendData(so, pkt->getStreamData(), pkt->getStreamDataBytes());
// 				}
// 			}
		}

		void TCPSession::receivedDataNotification(
			const unsigned char* data /* = nullptr */,
			const int bytes /* = 0 */)
		{
			if (so && so->is_open() && receiverPtr)
			{
				receiverPtr->recvData(so);
			}
		}
		
		void TCPSession::receivedExceptionNotification(const boost::system::error_code e)
		{
			stopSession();
		}

		void TCPSession::receivedExpiredNotification(void)
		{
			stopSession();
		}
	}//namespace network
}//namespace base
