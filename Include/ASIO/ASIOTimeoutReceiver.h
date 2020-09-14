//
//		Copyright :				@2017, HHJT, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@hotmail.com
//		Date :							2017-07-14
//		Description:				ASIO超时接收器
//
//		History:						Author									Date											Description
//											王科威									2017-07-14									创建
//

#ifndef ASIO_TIMEOUT_RECEIVER_H
#define ASIO_TIMEOUT_RECEIVER_H

#include "ASIOReceiver.h"
using ExpiredTimer = boost::asio::deadline_timer;
using TimerPtr = boost::shared_ptr<ExpiredTimer>;

NS_BEGIN(asio, 1)

class ASIOTimeoutReceiver final : public ASIOReceiver
{
public:
	ASIOTimeoutReceiver(ReceivedCallback callback = nullptr, Handle ctx = nullptr, const UInt16 expire = 5/*second(s)*/);
	~ASIOTimeoutReceiver(void);

protected:
	int posting(Socket s = nullptr) override;

private:
	void expired(ErrorCode e, Socket s, TimerPtr timer);

private:
	const UInt16 secondsOfExpired;
};//class ASIOTimeoutReceiver

NS_END

#endif//ASIO_TIMEOUT_RECEIVER_H
