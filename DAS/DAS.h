//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-12-16
//		Description :					IPv4��ַ������
//
//		History:						Author									Date										Description
//										������									2020-12-16									����
//

#ifndef DAS_H
#define DAS_H

#include "Network/ASIO/Core/Asio.h"
using ASIO = framework::network::asio::ASIO;
#include "Network/ASIO/Session/UDPSession.h"
using UDPSession = framework::network::asio::UDPSession;

class DAS final : protected UDPSession
{
public:
	DAS(void);
	~DAS(void);

public:
	//����DAS
	//@recvPort : ���ݽ��ն˿ں�
	//@Return : ������
	int startDAS(const unsigned short recvPort = 0);

	//ֹͣDAS
	//@Return : ������
	int stopDAS(void);

	void afterReceivedDataNotification(
		const int e = 0,
		DataPtr data = nullptr,
		const char* ipv4 = nullptr,
		const unsigned short port = 0) override;
// 	void afterCatchExceptionNotification(
// 		const int e = 0) override;
// 	void afterCatchTimeoutdNotification(void) override;

private:
	ASIO asio;
	void* so;
};//class DAS

#endif//DAS_H
