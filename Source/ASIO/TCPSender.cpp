// #include "boost/asio.hpp"
#include "boost/bind.hpp"
// #include "boost/thread/lock_guard.hpp"
// #include "boost/make_shared.hpp"
// #include "boost/numeric/conversion/cast.hpp"
#include "ASIO/TCPSender.h"

NS_BEGIN(asio, 2)

TCPSender::TCPSender()
//	: sendDataNotificationCallback{ callback }, sendDataNotificationCtx{ ctx }
{}

TCPSender::~TCPSender()
{}

void TCPSender::asyncSend(
	boost::asio::ip::tcp::socket* so /* = nullptr */, const char* data /* = nullptr */, const Int32 bytes /* = 0 */, AfterSendDataNotificationCallback callback /* = nullptr */)
{
//	Int32 status{ ERR_BAD_OPERATE };

	if (so && so->is_open())
	{
// 		auto tempSenderSelf(boost::enable_shared_from_this<TCPSender>::shared_from_this());
// 		boost::asio::async_write(*so, boost::asio::buffer(data, bytes),
// 			[this, tempSenderSelf](boost::system::error_code ec, std::size_t bytes)
// 		{
// 		});

// 		boost::asio::async_write(
// 			*so, 
// 			boost::asio::buffer(data, bytes),
// 			boost::bind(
// 				&TCPSender::afterSendDataNotificationCallback, 
// 				boost::enable_shared_from_this<TCPSender>::shared_from_this(),
// 				so,
// 				boost::asio::placeholders::error,
// 				boost::asio::placeholders::bytes_transferred));
//		status = ERR_OK;

		boost::asio::async_write(*so, boost::asio::buffer(data, bytes), callback);
	}

//	return status;
}

// Int32 TCPSender::sendPartialData(
// 	boost::asio::ip::tcp::socket* so /* = nullptr */, const char* data /* = nullptr */, const Int32 bytes /* = 0 */)
// {
// 	Int32 status{ ERR_BAD_OPERATE };
// 
// 	if (so && so->is_open())
// 	{
// 	 	so->async_write_some(
// 			boost::asio::buffer(data, bytes),
// 	 		boost::bind(
// 	 			&TCPSender::afterSendDataNotificationCallback,
// 	 			boost::enable_shared_from_this<TCPSender>::shared_from_this(),
// 	 			so,
// 	 			boost::asio::placeholders::error,
// 	 			boost::asio::placeholders::bytes_transferred));
// 	}
// 
// 	return ERR_OK;
// }

// void TCPSender::afterSendDataNotificationCallback(
// 	boost::asio::ip::tcp::socket* so, 
// 	boost::system::error_code error, 
// //	boost::shared_ptr<boost::asio::streambuf> streamBuf, 
// 	const std::size_t transfferredBytes /* = 0 */)
// {
// // 	if (so && streamBuf && !error.value())
// // 	{
// // 		streamBuf->consume(transfferedBytes);
// // 	}
// 
// 	if (sendDataNotificationCallback)
// 	{
// 		sendDataNotificationCallback(so, error.value(), static_cast<Int32>(transfferredBytes), sendDataNotificationCtx);
// 	}
// }

NS_END
