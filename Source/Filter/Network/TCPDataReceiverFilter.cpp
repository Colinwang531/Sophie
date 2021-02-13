#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "Error.h"
#include "Time/XTime.h"
using Time = framework::time::Time;
#include "Pin/Pin.h"
#include "Data/Buffer.h"
#include "Network/ASIO/Session/TCPSourceSession.h"
using TCPSourceSession = framework::network::asio::TCPSourceSession;
using TCPSourceSessionPtr = boost::shared_ptr<TCPSourceSession>;
#include "Filter/Network/TCPDataReceiverFilter.h"

namespace framework
{
	namespace multimedia
	{
		TCPDataReceiverFilter::TCPDataReceiverFilter()
			: Filter(FilterType::FILTER_TYPE_SOURCE)
		{
			Filter::addPin(OutputPinName);
		}
		TCPDataReceiverFilter::~TCPDataReceiverFilter()
		{}

		int TCPDataReceiverFilter::start(
			const std::string url, 
			boost::asio::ip::tcp::socket& so)
		{
			int e{ !url.empty() ? eSuccess : eInvalidParameter };
			
			if (eSuccess == e)
			{
				if (!url.empty())
				{
					const size_t pos{ url.find_first_of(':') };
					const std::string xmsIP{ url.substr(0, pos) };
					const std::string streamurl{ url.substr(pos + 1, url.length()) };

					//TODO :
					//XMS的端口号默认为60531
					//这个需要在后期将端口配置到URL中
					TCPSessionPtr tsp{ 
						boost::make_shared<TCPSourceSession>(
							streamurl,
							so,
							boost::bind(&TCPDataReceiverFilter::afterTCPSendResultCallback, this, _1, _2),
							boost::bind(&TCPDataReceiverFilter::afterTCPReceiveResultCallback, this, _1, _2, _3)) };
					if (tsp)
					{
						TCPSourceSessionPtr source{ 
							boost::dynamic_pointer_cast<TCPSourceSession>(tsp) };
						e = source->connect(xmsIP.c_str(), 60531);

						if (eSuccess == e)
						{
							sp.swap(tsp);
						}
					}
					else
					{
						e = eBadNewObject;
					}
				}
				else
				{
					e = eObjectExisted;
				}
			}

			return e;
		}

		void TCPDataReceiverFilter::afterTCPSendResultCallback(
			const int e /* = 0 */,
			const int bytes /* = 0 */)
		{}

		void TCPDataReceiverFilter::afterTCPReceiveResultCallback(
			const int e /* = 0 */,  
			const void* data /* = nullptr */, 
			const int bytes /* = 0 */)
		{
			if (!e && data && 0 < bytes)
			{
				DataPtr dp{ boost::make_shared<Data>() };

				if (data && eSuccess == dp->setData(data, bytes))
				{
					Filter::inputData(dp);
				}
			}
		}
	}//namespace multimedia
}//namespace framework
