#include "boost/make_shared.hpp"
#include "Error.h"
#include "Session/TCPStreamTargetSession.h"

namespace base
{
	namespace network
	{
		TCPStreamTargetSession::TCPStreamTargetSession(
			boost::asio::ip::tcp::socket* s /* = nullptr */)
			: TCPSession(s)
		{}

		TCPStreamTargetSession::~TCPStreamTargetSession()
		{}

		void TCPStreamTargetSession::receivedDataNotification(
			const unsigned char* data /* = nullptr */, const int bytes /* = 0 */)
		{
			TCPSession::receivedDataNotification(data, bytes);
		}
	}//namespace network
}//namespace base
