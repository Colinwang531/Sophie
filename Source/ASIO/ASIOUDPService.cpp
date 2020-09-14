#include "Network/UDPService.h"

namespace network_1
{
	UDPService::UDPService() : GeneralService(), startPortNum(gMinPort)
	{}

	UDPService::~UDPService()
	{}

	boost::uint16_t UDPService::nextPortNum(void)
	{
		Locker locker(portNumMtx);

		if (gMaxPort == startPortNum)
		{
			startPortNum = gMinPort;
		}

		while (1)
		{
			boost::asio::ip::udp::endpoint ep(boost::asio::ip::address_v4::any(), startPortNum);
			boost::asio::io_service ios;
			boost::asio::ip::udp::socket so(ios, ep.protocol());

			boost::system::error_code ec;
			so.bind(ep, ec);
			if (!ec) 
			{
				boost::uint16_t result = startPortNum;
				++startPortNum;
				return result;
			}
		}
	}

}//namespace network_1
